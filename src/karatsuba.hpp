#ifndef KARATSUBA_HPP
#define KARATSUBA_HPP

#include "numeros.hpp"

// a * b por Karatsuba (tres productos recursivos), Θ(n^log2(3)).
// Precondición: a y b válidos (esNumeroValido). Resultado normalizado.
Num multiplicarKaratsuba(const Num& a, const Num& b);

#endif