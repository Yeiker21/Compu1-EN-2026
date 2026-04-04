#include "macondian.h"
#include <cmath> // Para fabs (valor absoluto)

// Variables globales para mantener el estado del sistema
int num_sensores = 0;
Result resultados_finales;
double RADIO_TOLERANCIA = 0.2;

void start(int n) {
    num_sensores = n;
    // Preparamos el contenedor de resultados: una serie para cada sensor
    resultados_finales.assign(n, MacondianSeries());
}

void batch(const Sensors& sensors) {
    // Procesamos cada sensor del lote
    for (int i = 0; i < num_sensores; ++i) {
        const MacondianSensor& microsensores = sensors[i];
        int m = microsensores.size();
        
        double suma_validos = 0.0;
        int conteo_validos = 0;

        // Aplicamos el modelo estadístico Macondiano para cada microsensor
        for (int j = 0; j < m; ++j) {
            double valor_actual = microsensores[j];
            
            // 1. Calcular promedio de los "otros"
            double suma_otros = 0.0;
            for (int k = 0; k < m; ++k) {
                if (k != j) suma_otros += microsensores[k];
            }
            double promedio_otros = suma_otros / (m - 1);

            // 2. Comparar con el radio de tolerancia (0.2)
            if (std::fabs(valor_actual - promedio_otros) <= RADIO_TOLERANCIA) {
                suma_validos += valor_actual;
                conteo_validos++;
            }
        }

        // 3. El valor Macondiano es el promedio de los no excluidos
        double macondo = 0.0;
        if (conteo_validos > 0) {
            macondo = suma_validos / conteo_validos;
        } else {
            // Si todos fueron excluidos (caso raro), promediamos todo el arreglo
            double suma_total = 0.0;
            for(double v : microsensores) suma_total += v;
            macondo = suma_total / m;
        }

        // Guardamos el resultado en la serie correspondiente al sensor i
        resultados_finales[i].push_back(macondo);
    }
}

const Result& final() {
    return resultados_finales;
}
