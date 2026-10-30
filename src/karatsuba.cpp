#include "karatsuba.hpp"

#include <algorithm>
#include <cassert>

#include "clasica.hpp"

namespace {

// Con n <= 3 se usa la clásica. Para n >= 4 los subproblemas
// (mitades y sumas de mitades, hasta ceil(n/2)+1 dígitos) son estrictamente menores que n.
constexpr std::size_t UMBRAL = 3;

// Precondición: a y b válidos. Pueden tener longitudes distintas.
Num karatsubaRec(const Num& a, const Num& b) {
    // Atajo: multiplicar por cero no necesita recursión
    if (a == "0" || b == "0") return "0";

    const std::size_t n = std::max(a.size(), b.size());

    // Caso base: números chicos, se resuelven con la clásica
    if (n <= UMBRAL) return multiplicarClasica(a, b);

    // Partir ambos números en la misma posición m
    const std::size_t m = n / 2;
    Num x1, x0, y1, y0;
    partir(a, m, x1, x0);
    partir(b, m, y1, y0);
    x1 = normalizar(x1);
    x0 = normalizar(x0);
    y1 = normalizar(y1);
    y0 = normalizar(y0);

    // Los tres productos de Karatsuba
    const Num p1 = karatsubaRec(x1, y1);
    const Num p2 = karatsubaRec(x0, y0);
    const Num p3 = karatsubaRec(sumar(x1, x0), sumar(y1, y0));

    // Término central: (P3 - P1) - P2, en este orden para no tener negativos
    const Num medio = restar(restar(p3, p1), p2);

    // Recomposición: P1*10^(2m) + medio*10^m + P2
    return sumar(sumar(desplazar(p1, 2 * m), desplazar(medio, m)), p2);
}

}  // namespace

Num multiplicarKaratsuba(const Num& a, const Num& b) {
    assert(esNumeroValido(a) && esNumeroValido(b));

    Num x = normalizar(a);
    Num y = normalizar(b);

    if (x == "0" || y == "0") return "0";

    rellenarPotencia2(x, y);
    return normalizar(karatsubaRec(x, y));
}