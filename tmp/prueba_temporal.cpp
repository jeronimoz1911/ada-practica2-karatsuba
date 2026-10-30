#include <iostream>
#include <random>

#include "../src/clasica.hpp"
#include "../src/karatsuba.hpp"
#include "../src/numeros.hpp"

// Compara Karatsuba contra la clásica y reporta si coinciden
static bool probar(const Num& a, const Num& b) {
    const Num k = multiplicarKaratsuba(a, b);
    const Num c = multiplicarClasica(a, b);
    if (k != c) {
        std::cout << "FALLO: " << a << " x " << b << "\n"
                  << "  karatsuba: " << k << "\n"
                  << "  clasica:   " << c << "\n";
        return false;
    }
    return true;
}

int main() {
    int fallos = 0;

    // Casos de borde
    const char* par[][2] = {
        {"1234", "5678"},          // 7006652
        {"0000012", "3"},          // ceros a la izquierda
        {"0", "999"},              // cero
        {"99", "99"},              // 1-2 dígitos
        {"99999", "99999"},        // longitud impar, muchos 9
        {"1000007", "1000007"},    // ceros intercalados
        {"5", "123456"},           // longitudes muy distintas
    };
    for (auto& p : par) {
        if (!probar(p[0], p[1])) ++fallos;
    }

    // Aleatorios de 1 a 300 dígitos
    std::mt19937 gen(42);
    std::uniform_int_distribution<int> digito(0, 9);
    std::uniform_int_distribution<int> largo(1, 300);
    for (int t = 0; t < 500; ++t) {
        Num a, b;
        for (int i = largo(gen); i > 0; --i) a.push_back(static_cast<char>('0' + digito(gen)));
        for (int i = largo(gen); i > 0; --i) b.push_back(static_cast<char>('0' + digito(gen)));
        if (!probar(a, b)) ++fallos;
    }

    std::cout << (fallos == 0 ? "TODO CORRECTO\n" : "HAY FALLOS\n");
    return fallos == 0 ? 0 : 1;
}