import serial
import csv
from datetime import datetime

arduino = serial.Serial('COM3')
nombre = "ina219_gain2"
narchivo = 1
mediciones = 10

datos = []

arduino.write(b'0')

for i in range(mediciones):
    dato = arduino.readline()[:-2].decode('utf-8').split(',')
    print(dato)
    datos.append([datetime.now().strftime(format="%Y-%m-%d %H:%M:%S"),
                  float(dato[0]),
                  float(dato[1]),
                  float(dato[2]),
                  float(dato[3])])


# Guardar en CSV
with open(f"{nombre}_{narchivo}.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["timestamp", "corriente", "voltage", "potencia", "shunt"])  # encabezados
    writer.writerows(datos)

print("Datos guardados en mediciones.csv")
