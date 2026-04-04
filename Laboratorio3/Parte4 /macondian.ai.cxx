#include "macondian.h"
#include <cmath>

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
        double suma_validos = 0.0;
        int conteo_validos = 0;

        for (int j = 0; j < m; ++j) {
            double valor_actual = microsensores[j];
            double suma_otros = 0.0;
            for (int k = 0; k < m; ++k) {
                if (k != j) suma_otros += microsensores[k];
            }
            double promedio_otros = suma_otros / (m - 1);

            if (std::fabs(valor_actual - promedio_otros) <= RADIO_TOLERANCIA) {
                suma_validos += valor_actual;
                conteo_validos++;
            }
        }
        double macondo = (conteo_validos > 0) ? (suma_validos / conteo_validos) : 0.0;
        resultados_finales[i].push_back(macondo);
    }
}

const Result& final() { return resultados_finales; }
