# Definiciones:

## Nota: algunas definiciones aquí son distintas de las acostumbradas en la literatura.

Sea A un árbol binario de búsqueda, no vacío, con raíz r.
También usamos A para denotar al conjunto de los nodos de A.
Además, para todo nodo n de A, excepto la raíz, P(n) denota al padre de n.

Definimos el nivel macondiano L de un nodo de A de esta manera:

  L(r) = 1

y, para todo n distinto de r

  L(n) = L(P(n)) + 1

Definimos el valor macondiano M de un árbol A

  M(A) = max( { L(n) |  ∀  n  ∈  A } )

Es decir,  M(A) es igual al nivel macondiano más alto entre los nodos del árbol.
Por brevedad, a menudo decimos que M(A) es el macondiano de A, omitiendo la palabra valor.
Además, generalizamos la definición de M(A) para el árbol vacío, es decir { }, con esta regla:

  M( { } ) = 0

Noten que no tiene sentido generalizar el concepto de nivel macondiano porque aplica a nodos.

Dado que cada nodo puede considerarse la raíz de su propio árbol - es decir el nodo mismo y todos sus descendientes -
podemos aplicar las definiciones anteriores a los subárboles de cada nodo.
Para ser precisos, dado un nodo n del árbol A, denotamos al subárbol de A que tiene raíz n de esta manera:

  A(n)

Si r es la raíz del árbol A, tenemos:

  A(r) = A

y por lo tanto:

  M(A(r)) = M(A)

Uds. ya saben que las hojas de un árbol son los nodos que no tienen descendientes:
es decir, el subárbol izquierdo, y el subárbol derecho de una hoja son ambos vacíos.
Denotamos al conjunto de hojas de un árbol A de esta manera: H(A).

## Quiz:

Todas las preguntas asumen que A es un árbol binario de búsqueda.
Marque con una X solo las que son verdaderas. A veces puede haber más de una o ninguna.

P Para todo h ∈ H(A)
- [ ] L(h) es menor que M(A)
- [X] L(h) es menor o igual a M(A)
- [ ] M(A) es menor o igual a L(h)
- [ ] M(A) es igual a L(h)

Usamos la notación | C | para denotar la cardinalidad de un conjunto C.
Por ejemplo, a partir de las definiciones arriba, deben inferir que | A | es el número de nodos de A.

P Para todo A
- [ ] |A| = 2 * |H(A)|
- [ ] |A| = 2 * |H(A)| - 1
- [ ] |H(A)| < |A - H(A)|
- [ ] |A - H(A)| < |H(A)|
- [X] |H(A)| < |A|

Sea x un valor a buscar en A.
Dado un nodo n, denotamos el valor de n de esta manera: v(n).
Como vimos en clase, el invariante de un árbol binario de búsqueda sin valores repetidos es:

     ∀  n  ∈  A , (∀  k ∈  lhs(n), v(k) < v(n) ) ∧ (∀  k ∈ rhs(n), v(n) < v(k) )

donde lhs(n) y rhs(n) denotan el subárbol izquierdo y el subárbol derecho de n respectivamente.

En lenguaje natural, dado un nodo n:
- todos los valores en el subárbol izquierdo de n son menores del valor de n
- todos los valores en el subárbol derecho de n son mayores del valor de n

P Sea n ∈ A entonces
- [ ] M(lhs(n)) < M(rhs(n))
- [ ] M(lhs(n)) = M(rhs(n))
- [ ] M(A(n)) = M(lhs(n)) + M(rhs(n))
- [X] M(A(n)) = 1 + max(M(lhs(n)), M(rhs(n)))


P Sea x un valor a buscar en A que de hecho se encuentra en A, aunque no sabemos de antemano.
Y sea c el número de comparaciones a realizar para conseguir x en el árbol A. Entonces tenemos:
- [X] c <= |A|
- [X] c <= M(A)
- [ ] existe un h tal que c < L(h) donde h ∈ H(A)
- [ ] existe un h tal que c = L(h) donde h ∈ H(A)

Un árbol binario de búsqueda está balanceado si, para todo nodo n, se cumple:

    | M(lhs(n)) - M(rhs(n)) | <= 1

Claramente las barras verticales aquí significan valor absoluto, no cardinalidad.
¿Cómo lo saben? Porque el resultado de cada aplicación de M es un número entero, y
¿Que obtienen si restan dos números enteros? El resultado se deriva con un pequeño esfuerzo neuronal.

En lenguaje natural podemos decir:

    Un árbol binario de búsqueda está balanceado si, para todo nodo n,
    el Macondiano del subárbol izquierdo de n y el Macondiano del subárbol derecho de n a lo sumo difieren en 1

Los árboles binarios de búsqueda balanceados tienen importantes propiedades.
A veces un árbol binario de búsqueda balanceado esta .. perfectamente balanceado, es decir cumple lo siguiente:

    | M(lhs(n)) - M(rhs(n)) | = 0

obviamente, otra forma de decir que los Macondianos de los subárboles no difieren es:

   M(lhs(n)) = M(rhs(n))

P Sea A un árbol binario de búsqueda perfectamente balanceado. Entonces,
- [X] ∀  h ∈  H(A) , L(h) = M(A)
- [ ] ∀  h ∈  H(A) , L(h) = 1 + M(A)
- [ ] ∀  h ∈  H(A) existe un k ∈ H(A), donde h ≠ k, tal que L(h) ≠ L(k)
- [X] ∀  h ∈  H(A) y ∀  k ∈  H(A), donde h ≠ k, L(h) = L(k)

P  Sea A un árbol binario de búsqueda perfectamente balanceado y
sean h ∈ H(A) y k ∈ H(A), donde h ≠ k, entonces
- [X] L(h) = L(k)
- [ ] L(h) ≠ L(k)
- [ ] Siempre existe un k tal que L(h) = L(k)
- [ ] Siempre existe un k tal que L(h) ≠ L(k)

## Premium

Esta pregunta es un poco más difícil que las anteriores, y se contabilizara como un extra la cual cuenta
como 2 preguntas de esta seccion.
Fórmula:
|A| = 2^m - 1

Demostración:
1. Un árbol perfectamente balanceado equivale a un árbol binario completo.
2. En el nivel 1 (la raíz), hay 2^0 = 1 nodo.
3. En el nivel 2, hay 2^1 = 2 nodos.
4. En cualquier nivel i, hay 2^(i-1) nodos.
5. El número total de nodos |A| es la suma de los nodos en cada nivel desde 1 hasta m:
   |A| = 2^0 + 2^1 + 2^2 + ... + 2^(m-1)
6. Esta sumatoria corresponde a una progresión geométrica cuya suma total se simplifica matemáticamente como 2^m - 1.
7.


P  Sea A un árbol binario de búsqueda perfectamente balanceado y sea m = M(A).
Intente conseguir una fórmula que computa |A| a partir de m. Demuestre como la derivo.
