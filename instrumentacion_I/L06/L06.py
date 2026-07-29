# Laboratorio 06: Medidas de Propiedades Opticas
# Script para mediciones con LDR a diferentes distancias.

import csv
from datetime import datetime
import statistics
import time

import matplotlib.pyplot as plt
import serial

# Configuracion del puerto serial
PORT = "COM3"  # Cambie al puerto donde esta conectado el Arduino
BAUD_RATE = 9600
TIMEOUT = 2

# Configuracion de la medicion
DISTANCES_CM = [5, 10, 15, 20, 30, 50]
SAMPLES_PER_DISTANCE = 20


def calculate_resistance(adc_value, ref_resistance=10000, vcc=5):
    """Calcule la resistencia del LDR a partir de una lectura ADC."""
    if adc_value <= 0:
        return float("inf")

    voltage_out = (adc_value / 1023.0) * vcc
    if voltage_out <= 0:
        return float("inf")

    voltage_in = vcc - voltage_out
    return ref_resistance * (voltage_in / voltage_out)


def read_sensor_line(ser):
    """Lea una linea valida 'adc,timestamp' desde Arduino."""
    while True:
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if not line:
            continue

        parts = line.split(",")
        if len(parts) != 2:
            continue

        try:
            adc_value = int(parts[0])
            timestamp_ms = int(parts[1])
            return adc_value, timestamp_ms
        except ValueError:
            continue


def collect_samples(ser, distance_cm):
    """Tome el numero configurado de muestras para una distancia."""
    samples = []
    ser.reset_input_buffer()

    while len(samples) < SAMPLES_PER_DISTANCE:
        adc_value, _ = read_sensor_line(ser)
        samples.append(adc_value)
        print(
            f"d = {distance_cm:2d} cm | "
            f"{len(samples):02d}/{SAMPLES_PER_DISTANCE} | "
            f"ADC = {adc_value:4d}"
        )

    average = statistics.fmean(samples)
    std_dev = statistics.stdev(samples) if len(samples) > 1 else 0.0

    return {
        "distance_cm": distance_cm,
        "adc_average": average,
        "adc_std_dev": std_dev,
        "resistance_ohm": calculate_resistance(average),
        "samples": samples,
    }


def update_plots(results):
    """Actualice las graficas lineal y log-log con los resultados actuales."""
    distances = [result["distance_cm"] for result in results]
    averages = [result["adc_average"] for result in results]

    plt.clf()

    ax_linear = plt.subplot(1, 2, 1)
    ax_linear.plot(distances, averages, "o-", color="tab:blue")
    ax_linear.set_xlabel("Distancia (cm)")
    ax_linear.set_ylabel("Valor ADC promedio")
    ax_linear.set_title("Escala lineal")
    ax_linear.grid(True)

    ax_log = plt.subplot(1, 2, 2)
    positive_points = [
        (distance, average)
        for distance, average in zip(distances, averages)
        if average > 0
    ]
    if positive_points:
        log_distances, log_averages = zip(*positive_points)
        ax_log.loglog(log_distances, log_averages, "o-", color="tab:red")
    ax_log.set_xlabel("Distancia (cm)")
    ax_log.set_ylabel("Valor ADC promedio")
    ax_log.set_title("Escala log-log")
    ax_log.grid(True, which="both")

    plt.tight_layout()
    plt.pause(0.1)


def save_results(results):
    """Guarde el resumen de mediciones en un archivo CSV."""
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    filename = f"light_distance_data_{timestamp}.csv"

    with open(filename, "w", newline="") as csvfile:
        fieldnames = [
            "distance_cm",
            "adc_average",
            "adc_std_dev",
            "resistance_ohm",
            "samples",
        ]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

        for result in results:
            writer.writerow(
                {
                    "distance_cm": result["distance_cm"],
                    "adc_average": f"{result['adc_average']:.2f}",
                    "adc_std_dev": f"{result['adc_std_dev']:.2f}",
                    "resistance_ohm": f"{result['resistance_ohm']:.2f}",
                    "samples": " ".join(str(sample) for sample in result["samples"]),
                }
            )

    return filename


def main():
    results = []

    try:
        with serial.Serial(PORT, BAUD_RATE, timeout=TIMEOUT) as ser:
            print(f"Conectado a {PORT} a {BAUD_RATE} baudios")
            time.sleep(2)
            ser.reset_input_buffer()

            plt.ion()
            plt.figure(figsize=(10, 4))

            for distance_cm in DISTANCES_CM:
                input(
                    f"\nColoque el LDR a {distance_cm} cm de la fuente de luz "
                    "y presione Enter para medir."
                )
                result = collect_samples(ser, distance_cm)
                results.append(result)
                print(
                    f"Promedio: {result['adc_average']:.2f} ADC | "
                    f"Desv. est.: {result['adc_std_dev']:.2f} ADC | "
                    f"Resistencia: {result['resistance_ohm']:.0f} Ohm"
                )
                update_plots(results)

    except KeyboardInterrupt:
        print("\nMedicion detenida por el usuario.")
    except Exception as exc:
        print(f"Error: {exc}")

    if results:
        filename = save_results(results)
        print(f"\nDatos guardados como: {filename}")
        plt.ioff()
        plt.show()


if __name__ == "__main__":
    main()
