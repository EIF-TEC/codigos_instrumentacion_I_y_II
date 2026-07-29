import serial
import csv
from datetime import datetime

arduino = serial.Serial('COM8', 9600)
nombre = "term"
narchivo = 1
mediciones = 20

datos = []

arduino.write(b'0')

for i in range(mediciones):
    dato = arduino.readline()[:-2].decode('utf-8').split(',')
    print(dato)
    datos.append([datetime.now().strftime(format="%Y-%m-%d %H:%M:%S"),
                  float(dato[0]),
                  float(dato[1]),
                  float(dato[2])])


# Guardar en CSV
with open(f"{nombre}_{narchivo}.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["timestamp", "R", "T1", "T2"])  # encabezados
    writer.writerows(datos)

print("Datos guardados en el archivo .csv")