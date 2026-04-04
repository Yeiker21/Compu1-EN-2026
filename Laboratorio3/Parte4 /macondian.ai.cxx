#include "macondian.h"
#include <cmath> // Para fabs (valor absoluto)

// Variables globales
int num_sensores = 0;
Result resultados_finales;
double RADIO_TOLERANCIA = 0.2;

void start(int n) {
    num_sensores = n;
    resultados_finales.assign(n, MacondianSeries());
}

void batch(const Sensors& sensors) {
    for (int i = 0; i < num_sensores; ++i) {
        const MacondianSensor& microsensores = sensors[i];
        int m = microsensores.size();
        
        // 1. Calculamos la suma total de TODOS los microsensores de una sola vez
        double suma_total = 0.0;
        for (double v : microsensores) {
            suma_total += v;
        }

        double suma_validos = 0.0;
        int conteo_validos = 0;

        // 2. Comparamos y filtramos súper rápido
        for (int j = 0; j < m; ++j) {
            double valor_actual = microsensores[j];
            
            // Truco matemático: restamos el valor actual a la suma total
            double promedio_otros = (suma_total - valor_actual) / (m - 1);

            if (std::fabs(valor_actual - promedio_otros) <= RADIO_TOLERANCIA) {
                suma_validos += valor_actual;
                conteo_validos++;
            }
        }

        // 3. Calculamos el Macondo final y lo guardamos
        double macondo = 0.0;
        if (conteo_validos > 0) {
            macondo = suma_validos / conteo_validos;
        } else {
            macondo = suma_total / m;
        }

        resultados_finales[i].push_back(macondo);
    }
}

const Result& final() {
    return resultados_finales;
}
