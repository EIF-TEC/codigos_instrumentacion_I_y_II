import serial
import csv
from datetime import datetime

arduino = serial.Serial('COM9')
nombre = "scale900_800"
narchivo = 3
mediciones = 20

datos = []

arduino.write(b'0')

for i in range(mediciones):
    dato = arduino.readline()[:-2].decode('utf-8').split(',')
    print(dato)
    datos.append([datetime.now().strftime(format="%Y-%m-%d %H:%M:%S"),
                  float(dato[0]),
                  float(dato[1])
                  ])


# Guardar en CSV
with open(f"{nombre}_{narchivo}.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["timestamp", "raw", "peso"])  # encabezados
    writer.writerows(datos)

print("Datos guardados en mediciones.csv")