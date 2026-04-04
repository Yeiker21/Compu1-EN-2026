#include "macondian.h"
#include <cmath>

// Estado global para la versión optimizada
int total_sensores_ai = 0;
Result historia_macondiana_ai;
const double RADIO_TOLERANCIA = 0.2;

void start(int n) {
    total_sensores_ai = n;
    historia_macondiana_ai.assign(n, MacondianSeries());
}

void batch(const Sensors& sensors) {
    for (int i = 0; i < total_sensores_ai; ++i) {
        const MacondianSensor& micros = sensors[i];
        int m = micros.size();
        
        // ========================================================================
        // OPTIMIZACIÓN ALGORÍTMICA: 
        // En lugar de usar un ciclo anidado para sumar los "otros" sensores (lo 
        // cual nos daría una complejidad de O(N^2) que es costosa para el firmware),
        // pre-calculamos la suma total de todo el arreglo una sola vez en O(N).
        // ========================================================================
        double suma_total = 0.0;
        for (double v : micros) {
            suma_total += v;
        }

        double suma_filtrada = 0.0;
        int cantidad_validos = 0;

        // Filtramos los datos en tiempo lineal O(N)
        for (int j = 0; j < m; ++j) {
            double valor_actual = micros[j];
            
            // Truco algebraico: El promedio de los "otros" se deduce restando el 
            // valor actual a la suma total y dividiendo entre (m - 1).
            double promedio_otros = (suma_total - valor_actual) / (m - 1);

            // Verificamos si la desviación está dentro del umbral permitido
            if (std::fabs(valor_actual - promedio_otros) <= RADIO_TOLERANCIA) {
                suma_filtrada += valor_actual;
                cantidad_validos++;
            }
        }

        // Promedio de los que pasaron el filtro (o de la suma total si fue un evento atípico)
        double valor_final = (cantidad_validos > 0) ? (suma_filtrada / cantidad_validos) : (suma_total / m);
        
        historia_macondiana_ai[i].push_back(valor_final);
    }
}

const Result& final() {
    return historia_macondiana_ai;
}
