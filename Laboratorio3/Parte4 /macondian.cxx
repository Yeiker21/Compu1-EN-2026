#include "macondian.h"
#include <cmath> // Necesario para usar std::fabs (valor absoluto)

// Variables de estado global del sistema
int total_sensores = 0;
Result historia_macondiana;
const double RADIO_TOLERANCIA = 0.2; // Definido por el modelo del Dr. Solana

void start(int n) {
    total_sensores = n;
    // Inicializamos el contenedor de resultados: creamos 'n' series vacías, una para cada sensor
    historia_macondiana.assign(n, MacondianSeries());
}

void batch(const Sensors& sensors) {
    // Procesamos el lote (batch) recorriendo cada sensor reportado
    for (int i = 0; i < total_sensores; ++i) {
        const MacondianSensor& micros = sensors[i];
        int m = micros.size();
        
        double suma_filtrada = 0.0;
        int cantidad_validos = 0;

        // Evaluamos cada microsensor individualmente para ver si es confiable
        for (int j = 0; j < m; ++j) {
            double valor_actual = micros[j];
            
            // PASO 1: Calcular el promedio de los DEMÁS microsensores
            double suma_otros = 0.0;
            for (int k = 0; k < m; ++k) {
                if (k != j) {
                    suma_otros += micros[k];
                }
            }
            double promedio_otros = suma_otros / (m - 1);

            // PASO 2: Filtrar. Si la diferencia absoluta está dentro del radio de tolerancia, lo tomamos en cuenta
            if (std::fabs(valor_actual - promedio_otros) <= RADIO_TOLERANCIA) {
                suma_filtrada += valor_actual;
                cantidad_validos++;
            }
        }

        // PASO 3: El valor final del sensor Macondiano es el promedio de los microsensores válidos
        double valor_final = 0.0;
        if (cantidad_validos > 0) {
            valor_final = suma_filtrada / cantidad_validos;
        } else {
            // Caso extremo (Fallback): Si por alguna razón de ruido extremo todos fallan la tolerancia,
            // promediamos todos los valores para no dejar el sensor en cero ni perder la data bruta.
            double suma_total_rescate = 0.0;
            for (double v : micros) suma_total_rescate += v;
            valor_final = suma_total_rescate / m;
        }

        // Guardamos el resultado procesado en la serie histórica correspondiente a este sensor
        historia_macondiana[i].push_back(valor_final);
    }
}

const Result& final() {
    // Retornamos la estructura completa con todo el histórico de medidas
    return historia_macondiana;
}
