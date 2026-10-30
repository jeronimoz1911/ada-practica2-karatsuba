// Validacion de correctitud: Karatsuba contra la clasica

#include <iostream>
#include <random>
#include <string>

#include "../src/clasica.hpp"
#include "../src/karatsuba.hpp"
#include "../src/numeros.hpp"

static int pruebas = 0;
static int fallos = 0;

static void reportar(bool ok, const std::string& nombre) {
    pruebas++;
    if (!ok) fallos++;
    std::cout << (ok ? "[ OK ] " : "[FALLA] ") << nombre << "\n";
}

// Compara ambos algoritmos entre si y con el resultado esperado (si se da)
static void caso(const std::string& nombre, const Num& a, const Num& b,
                 const Num& esperado = "") {
    const Num c = multiplicarClasica(a, b);
    const Num k = multiplicarKaratsuba(a, b);
    const bool ok = (c == k) && (esperado.empty() || c == esperado);
    reportar(ok, nombre);
    if (!ok) {
        std::cout << "    a         = " << a << "\n"
                  << "    b         = " << b << "\n"
                  << "    clasica   = " << c << "\n"
                  << "    karatsuba = " << k << "\n";
        if (!esperado.empty()) std::cout << "    esperado  = " << esperado << "\n";
    }
}

static Num aleatorio(std::mt19937_64& rng, std::size_t n, bool primeroNoCero = true) {
    std::uniform_int_distribution<int> digito(0, 9), noCero(1, 9);
    Num s;
    for (std::size_t i = 0; i < n; i++) {
        const int d = (i == 0 && primeroNoCero) ? noCero(rng) : digito(rng);
        s.push_back(static_cast<char>('0' + d));
    }
    return s;
}

int main() {
    std::mt19937_64 rng(2026);

    std::cout << "== Casos de borde ==\n";
    caso("1 digito: 7 x 8", "7", "8", "56");
    caso("1 digito: 9 x 9", "9", "9", "81");
    caso("1 digito: 0 x 5", "0", "5", "0");
    caso("numero grande x 0", "123456789012345678901234567890", "0", "0");
    caso("ceros a la izquierda", "0000012", "0045", "540");
    caso("solo ceros", "0000", "000000", "0");
    caso("ceros intercalados", "100200300", "400050006", "40085130616201800");
    caso("ceros intercalados 2", "1000000001", "1000000001", "1000000002000000001");
    caso("longitud impar: 3 x 5 digitos", "123", "45678", "5618394");
    caso("longitud impar: 7 x 7 digitos", "1234567", "7654321", "9449772114007");
    caso("longitudes muy distintas", "5", "123456789", "617283945");
    caso("todo nueves (maximo acarreo)", "99999999", "99999999", "9999999800000001");
    caso("1234 x 5678", "1234", "5678", "7006652");

    std::cout << "\n== Productos grandes ==\n";
    caso("10^50 x 10^70", "1" + std::string(50, '0'), "1" + std::string(70, '0'),
         "1" + std::string(120, '0'));

    // (10^500 menos 1) al cuadrado da 499 nueves, un 8, 499 ceros y un 1
    {
        const Num nueves(500, '9');
        const Num esperado = std::string(499, '9') + "8" + std::string(499, '0') + "1";
        caso("500 nueves al cuadrado", nueves, nueves, esperado);
    }

    caso("aleatorio 300 x 300 digitos", aleatorio(rng, 300), aleatorio(rng, 300));
    caso("aleatorio 777 x 513 digitos", aleatorio(rng, 777), aleatorio(rng, 513));
    caso("aleatorio 2048 x 2048 digitos", aleatorio(rng, 2048), aleatorio(rng, 2048));

    std::cout << "\n== Pruebas aleatorias masivas ==\n";

    // Hasta 9 digitos: el producto cabe en unsigned long long y sirve de referencia
    {
        int f = 0;
        std::uniform_int_distribution<unsigned long long> d(0, 999999999ULL);
        for (int i = 0; i < 5000; i++) {
            const unsigned long long a = d(rng), b = d(rng);
            const std::string esperado = std::to_string(a * b);
            const Num sa = std::to_string(a), sb = std::to_string(b);
            if (multiplicarClasica(sa, sb) != esperado ||
                multiplicarKaratsuba(sa, sb) != esperado) {
                f++;
                if (f <= 3) std::cout << "    fallo con " << a << " x " << b << "\n";
            }
        }
        reportar(f == 0, "5000 pares de hasta 9 digitos vs unsigned long long");
    }

    {
        int f = 0;
        std::uniform_int_distribution<int> largo(1, 300);
        for (int i = 0; i < 2000; i++) {
            const Num a = aleatorio(rng, largo(rng), false);
            const Num b = aleatorio(rng, largo(rng), false);
            if (multiplicarKaratsuba(a, b) != multiplicarClasica(a, b)) {
                f++;
                if (f <= 3) std::cout << "    fallo con " << a << " x " << b << "\n";
            }
        }
        reportar(f == 0, "2000 pares aleatorios de 1 a 300 digitos");
    }

    std::cout << "\nResultado: " << (pruebas - fallos) << "/" << pruebas
              << " pruebas pasaron\n";
    return fallos == 0 ? 0 : 1;
}
