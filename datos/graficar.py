# Genera las graficas de tiempo a partir de datos/tiempos.csv
# Uso desde la raiz del repo: python datos/graficar.py

import csv
import math
import os
import sys

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

ruta = sys.argv[1] if len(sys.argv) > 1 else os.path.join("datos", "tiempos.csv")
carpeta = os.path.dirname(ruta) or "."

n, clasica, karatsuba = [], [], []
with open(ruta, newline="") as f:
    for fila in csv.DictReader(f):
        n.append(int(fila["n"]))
        clasica.append(float(fila["clasica_mediana_us"]))
        karatsuba.append(float(fila["karatsuba_mediana_us"]))

cruce = next((ni for ni, c, k in zip(n, clasica, karatsuba) if k < c), None)


# Pendiente en escala log-log de los ultimos puntos (minimos cuadrados)
def pendiente(xs, ys, puntos=4):
    lx = [math.log(x) for x in xs[-puntos:]]
    ly = [math.log(y) for y in ys[-puntos:]]
    mx, my = sum(lx) / len(lx), sum(ly) / len(ly)
    num = sum((a - mx) * (b - my) for a, b in zip(lx, ly))
    den = sum((a - mx) ** 2 for a in lx)
    return num / den


pc = pendiente(n, clasica)
pk = pendiente(n, karatsuba)

plt.figure(figsize=(8, 5))
plt.plot(n, [t / 1000 for t in clasica], "o-", label="Clásica  Θ(n²)")
plt.plot(n, [t / 1000 for t in karatsuba], "s-", label="Karatsuba  Θ(n^1.585)")
if cruce:
    plt.axvline(cruce, color="gray", linestyle=":", label=f"Cruce en n = {cruce}")
plt.xlabel("Número de dígitos (n)")
plt.ylabel("Tiempo por multiplicación (ms)")
plt.title("Clásica vs Karatsuba (escala lineal)")
plt.grid(alpha=0.3)
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(carpeta, "grafica_lineal.png"), dpi=150)

plt.figure(figsize=(8, 5))
plt.loglog(n, clasica, "o-", base=2, label=f"Clásica (pendiente medida ≈ {pc:.2f})")
plt.loglog(n, karatsuba, "s-", base=2, label=f"Karatsuba (pendiente medida ≈ {pk:.2f})")

# Rectas de referencia con las pendientes teoricas
ref2 = [clasica[-1] * (x / n[-1]) ** 2 for x in n]
ref158 = [karatsuba[-1] * (x / n[-1]) ** math.log2(3) for x in n]
plt.loglog(n, ref2, ":", color="C0", alpha=0.5, base=2, label="Referencia pendiente 2")
plt.loglog(n, ref158, ":", color="C1", alpha=0.5, base=2, label="Referencia pendiente log₂3 ≈ 1.585")

if cruce:
    plt.axvline(cruce, color="gray", linestyle=":", label=f"Cruce en n = {cruce}")
plt.xlabel("Número de dígitos (n)")
plt.ylabel("Tiempo por multiplicación (µs)")
plt.title("Clásica vs Karatsuba (escala log log)")
plt.grid(alpha=0.3, which="both")
plt.legend(fontsize=8)
plt.tight_layout()
plt.savefig(os.path.join(carpeta, "grafica_loglog.png"), dpi=150)

print(f"Pendiente medida (ultimos 4 puntos): clasica = {pc:.3f}, karatsuba = {pk:.3f}")
print(f"Punto de cruce: {cruce if cruce else 'no hubo en el rango'}")
print(f"Graficas guardadas en {carpeta}")
