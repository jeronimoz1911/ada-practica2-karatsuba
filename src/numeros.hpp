#ifndef NUMEROS_HPP
#define NUMEROS_HPP

#include <cstddef>
#include <string>

// Representación: entero no negativo en base 10 como std::string.
// Solo caracteres '0'..'9', dígito MÁS significativo primero (como se escribe).
// Ejemplo: 4075 -> "4075". Se permiten ceros a la izquierda ("0042").
// El cero se representa como "0".
using Num = std::string;

// true si a no está vacío y todos sus caracteres son dígitos.
bool esNumeroValido(const Num& a);

// Quita ceros a la izquierda; si no queda nada devuelve "0".
Num normalizar(const Num& a);

// Antepone ceros hasta que a tenga longitud n. Si ya tiene n o más, la deja igual.
Num rellenar(const Num& a, std::size_t n);

// Menor potencia de 2 >= n (con siguientePotencia2(0) == 1).
// Lanza std::overflow_error si esa potencia no cabe en size_t.
std::size_t siguientePotencia2(std::size_t n);

// Rellena a y b con ceros a la izquierda hasta una misma longitud,
// que es una potencia de 2. Devuelve esa longitud.
std::size_t rellenarPotencia2(Num& a, Num& b);

// a + b. Admite longitudes distintas. Resultado normalizado.
Num sumar(const Num& a, const Num& b);

// a - b. Precondición: a >= b. Resultado normalizado.
Num restar(const Num& a, const Num& b);

// a * 10^k (añade k ceros a la derecha). Si a es "0" devuelve "0".
Num desplazar(const Num& a, std::size_t k);

// Separa a en alta y baja, donde baja son los últimos m dígitos y alta el resto.
// Si m >= longitud de a: alta = "0", baja = a. Si m == 0: alta = a, baja = "0".
// No normaliza: las mitades conservan sus ceros a la izquierda.
void partir(const Num& a, std::size_t m, Num& alta, Num& baja);

enum class Comparacion {MENOR,IGUAL,MAYOR}; // Evitar repetir logica de comparación en varios sitios.
Comparacion comparar(const Num& a, const Num& b);

#endif