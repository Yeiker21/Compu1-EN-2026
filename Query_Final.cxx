///
/// Query.cxx - archivo para completar los ejercicios del examen
///

#include "Query.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "Estudiante.h"
#include "Calificacion.h"

// q0: Calcula el promedio de todas las notas en la universidad
float Query::q0() {
    float suma = 0;
    int total = 0;
    
    for (const auto& c : Calificacion::tabla()) {
        suma += c.nota();
        total++;
    }
    
    if (total == 0) return 0;
    return suma / total;
}

// q1: Retorna un vector con todos los estudiantes activos
std::vector<Estudiante> Query::q1() {
    std::vector<Estudiante> resultado;
    
    for (const auto& e : Estudiante::tabla()) {
        if (e.activo()) {
            resultado.push_back(e);
        }
    }
    
    return resultado;
}

// q2: Cuenta cuántos estudiantes se han graduado
int Query::q2() {
    int graduados = 0;
    
    for (const auto& e : Estudiante::tabla()) {
        if (e.graduado()) {
            graduados++;
        }
    }
    
    return graduados;
}
