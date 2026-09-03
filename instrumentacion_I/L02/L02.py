import serial
import numpy as np
import matplotlib.pyplot as plt

arduino = serial.Serial('COM8', 9600)
fs = 10          # muestras por segundo
duracion = 20     # segundos
mediciones = fs * duracion  # 200 lecturas

datos = {"R": [], "T1": []}

arduino.write(b'0')

for i in range(mediciones):
    dato = arduino.readline()[:-2].decode('utf-8').split(',')
    print(dato)
    datos["R"].append(float(dato[0]))
    datos["T1"].append(float(dato[1]))

tiempo = np.arange(mediciones) / fs

for variable, valores in datos.items():
    valores = np.array(valores)
    promedio = valores.mean()
    desv_std = valores.std()
    print(f"{variable}: promedio = {promedio:.4f}, desviación estándar = {desv_std:.4f}")

    plt.figure()
    plt.plot(tiempo, valores, '-', label="Lecturas")
    plt.axhline(promedio, color='r', linestyle='--', label="Promedio")
    plt.fill_between(tiempo, promedio - desv_std, promedio + desv_std,
                      color='r', alpha=0.2, label="Promedio ± desv. estándar")
    plt.xlabel("Tiempo (s)")
    plt.ylabel(variable)
    plt.title(f"{variable}: {mediciones} lecturas en {duracion} s ({fs} muestras/s)")
    plt.legend()

plt.show()
