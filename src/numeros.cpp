#include "numeros.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>

bool esNumeroValido(const Num& a) {
    if (a.empty()) return false;
    for (char c : a) {
        if (c < '0' || c > '9') return false;
    }
    return true;
} // true si a no está vacío y todos sus caracteres son dígitos.

Num normalizar(const Num& a) {
    if (a.empty()) return "0";
    std::size_t i = 0;
    while (i + 1 < a.size() && a[i] == '0') ++i;
    return a.substr(i);
} // Quita ceros a la izquierda; si no queda nada devuelve "0".

Num rellenar(const Num& a, std::size_t n) {
    if (a.size() >= n) return a;
    return Num(n - a.size(), '0') + a;
} // Antepone ceros hasta que a tenga longitud n. Si ya tiene n o más, la deja igual.

std::size_t siguientePotencia2(std::size_t n) {
    const std::size_t limite = std::numeric_limits<std::size_t>::max() / 2;
    std::size_t p = 1;
    while (p < n) {
        if (p > limite) {
            throw std::overflow_error("siguientePotencia2: no hay potencia de 2 representable");
        }
        p <<= 1;
    }
    return p;
}
// Menor potencia de 2 >= n (con siguientePotencia2(0) == 1).
// Lanza std::overflow_error si esa potencia no cabe en size_t.

std::size_t rellenarPotencia2(Num& a, Num& b) {
    std::size_t mayor = std::max(a.size(), b.size());
    std::size_t n = siguientePotencia2(mayor);
    a = rellenar(a, n);
    b = rellenar(b, n);
    return n;
} // Rellena a y b con ceros a la izquierda hasta que tengan la misma longitud, que es una potencia de 2.

Num sumar(const Num& a, const Num& b) {
    std::size_t i = a.size();
    std::size_t j = b.size();
    int acarreo = 0;

    Num resultado;
    resultado.reserve(std::max(i, j) + 1);

    while (i > 0 || j > 0 || acarreo > 0) {
        int suma = acarreo;
        if (i > 0) suma += a[--i] - '0';
        if (j > 0) suma += b[--j] - '0';
        resultado.push_back(static_cast<char>('0' + suma % 10));
        acarreo = suma / 10;
    }

    std::reverse(resultado.begin(), resultado.end());
    return normalizar(resultado);
} // a + b. Admite longitudes distintas. Resultado normalizado.

Comparacion comparar(const Num& a, const Num& b) {
    std::size_t ia = 0, ib = 0;
    while (ia + 1 < a.size() && a[ia] == '0') ++ia;
    while (ib + 1 < b.size() && b[ib] == '0') ++ib;

    const std::size_t la = a.size() - ia;
    const std::size_t lb = b.size() - ib;
    if (la != lb) return la < lb ? Comparacion::MENOR : Comparacion::MAYOR;

    for (std::size_t k = 0; k < la; ++k) {
        if (a[ia + k] != b[ib + k]) {
            return a[ia + k] < b[ib + k] ? Comparacion::MENOR : Comparacion::MAYOR;
        }
    }
    return Comparacion::IGUAL;
} // Compara a y b, ignorando ceros a la izquierda. Devuelve MENOR, IGUAL o MAYOR.
// Es apoyo para restar y otras operaciones. No normaliza a ni b, pero ignora ceros a la izquierda.

Num restar(const Num& a, const Num& b) {
    assert(comparar(a, b) != Comparacion::MENOR);

    std::size_t i = a.size();
    std::size_t j = b.size();
    int prestamo = 0;

    Num resultado;
    resultado.reserve(i);

    while (i > 0) {
        int resta = (a[--i] - '0') - prestamo;
        if (j > 0) resta -= b[--j] - '0';

        if (resta < 0) {
            resta += 10;
            prestamo = 1;
        } else {
            prestamo = 0;
        }
        resultado.push_back(static_cast<char>('0' + resta));
    }

    std::reverse(resultado.begin(), resultado.end());
    return normalizar(resultado);
} // a - b. Precondición: a >= b. Resultado normalizado.

Num desplazar(const Num& a, std::size_t k) {
    if (a.find_first_not_of('0') == Num::npos) return "0";
    Num resultado = a;
    resultado.append(k, '0');
    return resultado;
} // a * 10^k (añade k ceros a la derecha). Si a es "0" devuelve "0".

void partir(const Num& a, std::size_t m, Num& alta, Num& baja) {
    Num h, l;
    if (m >= a.size()) {
        h = "0";
        l = a;
    } else if (m == 0) {
        h = a;
        l = "0";
    } else {
        const std::size_t corte = a.size() - m;
        h = a.substr(0, corte);
        l = a.substr(corte);
    }
    alta = std::move(h);
    baja = std::move(l);
} // Separa a en alta y baja, donde baja son los últimos m dígitos y alta el resto.

