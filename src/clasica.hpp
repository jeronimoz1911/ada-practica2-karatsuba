#ifndef CLASICA_HPP
#define CLASICA_HPP

#include "numeros.hpp"

// a * b por el método escolar: cada dígito de a por cada dígito de b, Θ(n·m).
// Precondición: a y b válidos (esNumeroValido). Resultado normalizado.
Num multiplicarClasica(const Num& a, const Num& b);

#endif