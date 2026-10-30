# Práctica 2: Multiplicación de Karatsuba

Análisis y Diseño de Algoritmos, Universidad EAFIT, 2026.2

Implementación en C++ de la multiplicación clásica Θ(n²) y del algoritmo de Karatsuba Θ(n^log₂3) ≈ Θ(n^1.585) para enteros grandes en base 10, con validación de correctitud y análisis experimental de tiempos.

## Integrantes

* Andrés Osorio
* Jerónimo Zapata
* Gisel Jaramillo

## Representación de los números

Cada número es un `std::string` (`using Num = std::string;`) con caracteres del `'0'` al `'9'` y el dígito más significativo primero, tal como se escribe: `4075` se guarda como `"4075"`. Se admiten ceros a la izquierda y el cero se representa como `"0"`. No se usan tipos nativos ni librerías externas para multiplicar.

## Estructura

```
src/      numeros, clasica y karatsuba (.hpp y .cpp)
tests/    tests.cpp, validación de correctitud
bench/    benchmark.cpp, medición de tiempos
datos/    tiempos.csv, graficar.py y las gráficas
informe/  informe técnico
```

## Requisitos

* `g++` con soporte para C++17
* Python 3 con `matplotlib`, solo para las gráficas (`pip install matplotlib`)

## Compilación

Desde la raíz del repositorio.

Windows:

```
g++ -std=c++17 -O2 tests\tests.cpp src\numeros.cpp src\clasica.cpp src\karatsuba.cpp -o tests_bin.exe
g++ -std=c++17 -O2 bench\benchmark.cpp src\numeros.cpp src\clasica.cpp src\karatsuba.cpp -o benchmark.exe
```

Linux o macOS:

```
g++ -std=c++17 -O2 tests/tests.cpp src/numeros.cpp src/clasica.cpp src/karatsuba.cpp -o tests_bin
g++ -std=c++17 -O2 bench/benchmark.cpp src/numeros.cpp src/clasica.cpp src/karatsuba.cpp -o benchmark
```

## Ejecución

**Pruebas de correctitud**

```
tests_bin.exe        (Linux o macOS: ./tests_bin)
```

Debe terminar con `Resultado: 20/20 pruebas pasaron`. Incluye números de un dígito, multiplicación por cero, ceros a la izquierda e intercalados, longitudes impares y distintas, máximo acarreo, productos de hasta 2048 dígitos, 5000 pares verificados contra `unsigned long long` y 2000 pares aleatorios de hasta 300 dígitos.

**Medición de tiempos**

```
benchmark.exe        (Linux o macOS: ./benchmark)
```

Mide ambos algoritmos para n = 2, 4, 8, ..., 4096 dígitos y escribe `datos/tiempos.csv`. Usa `std::chrono::steady_clock`, verifica que ambos algoritmos coincidan antes de medir, repite cada multiplicación hasta acumular 100 ms, toma la mediana de 5 mediciones y consume el resultado en una variable `volatile` para que el compilador no elimine el cálculo.

**Gráficas**

```
python datos/graficar.py
```

Genera `datos/grafica_lineal.png` y `datos/grafica_loglog.png`, e imprime la pendiente medida en escala log log y el punto de cruce.

## Uso de inteligencia artificial

Ver la declaración al final del informe.
