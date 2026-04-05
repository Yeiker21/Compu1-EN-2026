#include "macondian.h"
#include <cmath>

// Variables globales para guardar los datos
int n_sensores = 0;
Result resultados_finales;
const double TOLERANCIA = 0.2;

void start(int n) {
    n_sensores = n;
    // Preparamos el contenedor de resultados
    resultados_finales.assign(n, MacondianSeries());
}

void batch(const Sensors& sensors) {
    for (int i = 0; i < n_sensores; ++i) {
        const MacondianSensor& datos_sensor = sensors[i];
        int m = datos_sensor.size();
        
        // Calculo la suma de todos los valores del grupo una sola vez
        double suma_total = 0.0;
        for (double valor : datos_sensor) {
            suma_total += valor;
        }

        double suma_filtrada = 0.0;
        int sensores_validos = 0;

        // Filtro los datos comparando cada uno con el promedio de los demas
        for (int j = 0; j < m; ++j) {
            double valor_actual = datos_sensor[j];
            
            // Saco el promedio de los otros restando el actual de la suma total
            double promedio_otros = (suma_total - valor_actual) / (m - 1);

            if (std::abs(valor_actual - promedio_otros) <= TOLERANCIA) {
                suma_filtrada += valor_actual;
                sensores_validos++;
            }
        }

        // Si hay sensores validos saco su promedio, si no uso el promedio general
        double resultado_sensor = (sensores_validos > 0) ? (suma_filtrada / sensores_validos) : (suma_total / m);
        
        resultados_finales[i].push_back(resultado_sensor);
    }
}

const Result& final() {
    return resultados_finales;
}

// En este código logré mejorar el rendimiento haciendo el cálculo de la suma total al principio 
// y así evito un doble ciclo y el programa puede correr más rápido.
