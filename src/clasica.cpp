#include "clasica.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

#include "clasica.hpp"

#include <cassert>
#include <vector>

Num multiplicarClasica(const Num& a, const Num& b) {
    assert(esNumeroValido(a) && esNumeroValido(b));

    // Normalizar primero: los ceros a la izquierda no deben costar productos
    const Num x = normalizar(a);
    const Num y = normalizar(b);

    // Atajo: si alguno es 0, el producto es 0 y no se reserva nada
    if (x == "0" || y == "0") return "0";

    const std::size_t lx = x.size();
    const std::size_t ly = y.size();

    // Convertir y a dígitos una sola vez (en vez de restar '0' lx veces por dígito)
    std::vector<int> dy(ly);
    for (std::size_t j = 0; j < ly; ++j) dy[j] = y[j] - '0';

    std::vector<int> res(lx + ly, 0);

    // Acumular: cada par de dígitos aporta a una posición fija
    for (std::size_t i = lx; i > 0; --i) {
        const int dx = x[i - 1] - '0';
        if (dx == 0) continue;  // toda la fila aportaría 0
        for (std::size_t j = ly; j > 0; --j) {
            res[i + j - 1] += dx * dy[j - 1];
        }
    }

    // Propagar acarreos una sola vez, de derecha a izquierda
    for (std::size_t k = res.size() - 1; k > 0; --k) {
        res[k - 1] += res[k] / 10;
        res[k] %= 10;
    }

    Num resultado;
    resultado.reserve(res.size());
    for (int d : res) resultado.push_back(static_cast<char>('0' + d));
    return normalizar(resultado);
}