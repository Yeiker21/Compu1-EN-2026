#include <iostream>
#include "lab2.hpp"

int main() {
    // 1. Creamos una lista vacía
    List<int> l;
    
    // 2. Armamos la lista [1, 2, 3] como en las láminas
    // (Zilly dice que esto debe dar 6 al sumar)
    auto lista_prueba = l.cons(3).cons(2).cons(1); 

    // 3. Definimos la suma (nuestra función 'f')
    auto suma = [](int acc, int x) { return acc + x; };

    // 4. Llamamos a tu futura respuesta del Ejercicio 1
    int resultado = reduceLeft(suma, 0, lista_prueba);

    // 5. Mostramos el resultado en pantalla
    std::cout << "--- PRUEBA EJERCICIO 1 ---" << std::endl;
    std::cout << "Resultado obtenido: " << resultado << std::endl;
    std::cout << "Resultado esperado: 6" << std::endl;

    return 0;
}
