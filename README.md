# Prueba Técnica — Procesamiento de Flujo de Bytes en C++

## 1. Requisitos explícitos del problema


* 3 módulos diferentes:
  1. Generador
  2. Filtro
  3. Logger
* Interfaz común `IModule`.
* Cada módulo tiene un hilo independiente.
* El `main` no puede acceder a la definición de los módulos, solo a la interfaz `IModule`.

### 1. Generador
* Generará arrays de bytes de tamaño aleatorio (1 a 100) y con valores aleatorios (0 a 255).
* Generación continua.
* Entrega arrays a un input buffer.
* Velocidad de generación configurable.

### 2. Filtro
* Recibe arrays en un input buffer.
* Entrega arrays a un input buffer.
* Busca una secuencia específica de bytes en el array, esta secuencia puede ser de cualquier tamaño (1 a 100 bytes) y ser configurable.
* Solo entrega el array si encuentra un match de la secuencia objetivo.
* Tiene un input buffer con capacidad para 100 arrays.

### 3. Logger
* Recibe arrays en un input buffer.
* Cada vez que recibe un array debe registrarlo e imprimir TODOS los arrays recibidos por orden en hora UTC (no se indica en ningún momento que se imprima la hora).
* Tiene un input buffer con capacidad para 100 arrays (debe poder imprimir más de 100 arrays).

---

## 2. Suposiciones que he hecho

* Se utilizarán colas FIFO como input buffer.
* Parar un módulo implica esperar a que se acabe su buffer, procesamos todos los elementos pendientes antes de parar.
* El modo de funcionamiento de los módulos será continuo.
* Si el input buffer se llena, los datos que se intenten insertar se descartan y se informa al productor.
* Si un módulo tiene la cola vacía no debe consumir CPU.
* La parada de los módulos, tiene que ser ordenada (desde el generador hacia abajo) y respetando que los módulos vacíen su buffer. Esto queda como responsabilidad del consumidor de los modulos (main)
* La entrega de datos debe devolver el estado de esta (correcta, no aceptada por buffer lleno (consumidor no activo)).
* Secuencia de arranque (3, 2, 1) y secuencia de parada (1, 2, 3).

---

## 3. Dependencias que tenga el proyecto

* **C++20:**
* **GCC 13.3+**.
* **Linux**
* **CMake:** ( 3.20 o superior).

