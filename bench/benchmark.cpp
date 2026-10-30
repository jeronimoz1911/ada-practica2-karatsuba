// Medicion de tiempos: clasica vs Karatsuba para n = 2, 4, ..., 4096 digitos

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "../src/clasica.hpp"
#include "../src/karatsuba.hpp"
#include "../src/numeros.hpp"

using Reloj = std::chrono::steady_clock;

// volatile evita que el compilador elimine las multiplicaciones
static volatile std::size_t sumidero = 0;

static Num aleatorio(std::mt19937_64& rng, std::size_t n) {
    std::uniform_int_distribution<int> digito(0, 9), noCero(1, 9);
    Num s;
    s.reserve(n);
    s.push_back(static_cast<char>('0' + noCero(rng)));
    for (std::size_t i = 1; i < n; i++) s.push_back(static_cast<char>('0' + digito(rng)));
    return s;
}

// Repite la multiplicacion hasta acumular 100 ms y devuelve microsegundos por llamada
template <typename F>
static double unaMedicion(F f, const Num& a, const Num& b) {
    const double MIN_MS = 100.0;
    int reps = 0;
    double ms = 0.0;
    const auto inicio = Reloj::now();
    do {
        const Num r = f(a, b);
        sumidero = sumidero + r.size();
        reps++;
        ms = std::chrono::duration<double, std::milli>(Reloj::now() - inicio).count();
    } while (ms < MIN_MS);
    return ms * 1000.0 / reps;
}

struct Estadistica {
    double mediana;
    double promedio;
};

template <typename F>
static Estadistica medir(F f, const Num& a, const Num& b) {
    const int MEDICIONES = 5;
    sumidero = sumidero + f(a, b).size();  // calentamiento
    std::vector<double> t;
    for (int i = 0; i < MEDICIONES; i++) t.push_back(unaMedicion(f, a, b));
    std::sort(t.begin(), t.end());
    double suma = 0.0;
    for (double x : t) suma += x;
    return {t[MEDICIONES / 2], suma / MEDICIONES};
}

int main(int argc, char* argv[]) {
    const std::string ruta = (argc >= 2) ? argv[1] : "datos/tiempos.csv";
    std::ofstream csv(ruta);
    if (!csv) {
        std::cerr << "No se pudo abrir " << ruta << " (ejecuta desde la raiz del repo)\n";
        return 1;
    }

    std::mt19937_64 rng(42);

    csv << "n,clasica_mediana_us,clasica_promedio_us,"
           "karatsuba_mediana_us,karatsuba_promedio_us,razon_clasica_sobre_karatsuba\n";

    std::cout << "    n | clasica (us) | karatsuba (us) | clasica/karatsuba\n";

    std::size_t cruce = 0;
    for (std::size_t n = 2; n <= 4096; n *= 2) {
        const Num a = aleatorio(rng, n);
        const Num b = aleatorio(rng, n);

        if (multiplicarClasica(a, b) != multiplicarKaratsuba(a, b)) {
            std::cerr << "ERROR: los algoritmos no coinciden en n = " << n << "\n";
            return 1;
        }

        const Estadistica c = medir(multiplicarClasica, a, b);
        const Estadistica k = medir(multiplicarKaratsuba, a, b);
        const double razon = c.mediana / k.mediana;

        csv << n << "," << c.mediana << "," << c.promedio << ","
            << k.mediana << "," << k.promedio << "," << razon << "\n";

        std::printf("%5zu | %12.3f | %14.3f | %8.3f\n", n, c.mediana, k.mediana, razon);
        std::fflush(stdout);

        if (cruce == 0 && k.mediana < c.mediana) cruce = n;
    }

    std::cout << "\n";
    if (cruce) std::cout << "Punto de cruce: Karatsuba es mas rapido desde n = " << cruce << " digitos\n";
    else       std::cout << "No hubo punto de cruce en el rango 2..4096\n";
    std::cout << "Resultados guardados en " << ruta << "\n";
    return 0;
}
