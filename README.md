# codigos_instrumentacion_I_y_II

Código de las prácticas de laboratorio de **Instrumentación I** e **Instrumentación II**
(Escuela de Física, Tecnológico de Costa Rica): *sketches* de Arduino, scripts de Python
y archivos de simulación (Simulink/MATLAB) usados en las guías de laboratorio.

Este repositorio existe como fuente única de estos archivos para que las guías de
laboratorio de ambos cursos ([instrumentacion_I](https://github.com/EIF-TEC/instrumentacion_I),
[instrumentacion_II](https://github.com/EIF-TEC/instrumentacion_II)) los referencien desde
un solo lugar, en lugar de mantener copias duplicadas en cada repositorio.

## Estructura

```
instrumentacion_I/
    L02/ ... L06/     Código de cada práctica (Arduino .ino + Python .py)
    distancia/        Ejemplo adicional (sensor de distancia)
    BMP280_wire/      Ejemplo adicional (BMP280 por I2C)
    data.csv          Datos de ejemplo

instrumentacion_II/
    adc.py            Plantilla de ejemplo (ADC)
    maq_estados.py     Plantilla de ejemplo (máquina de estados)
    simulacion_t2/     Modelo de Simulink y script de MATLAB del taller de simulación
```

## Uso desde las guías de laboratorio

`instrumentacion_I/scripts/build_current_tex.sh` descarga automáticamente la carpeta
`instrumentacion_I/` de este repositorio a una carpeta local `code/` (ignorada por git)
la primera vez que se compila una guía que usa `\lstinputlisting`. Si ya existe una copia
local, no se vuelve a descargar; para forzar una actualización, borre la carpeta `code/`
local y vuelva a compilar.
