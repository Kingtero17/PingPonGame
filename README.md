# Pong CLI - C++ Terminal Game

Un clon del clásico juego Pong ejecutado íntegramente en la consola del sistema. Este proyecto implementa un motor de juego estructurado en C++ diseñado para operar en la terminal, con renderizado personalizado, menús en arte ASCII y captura de eventos en tiempo real.

## Interfaz de Juego

<img width="927" height="423" alt="image" src="https://github.com/user-attachments/assets/d365e7e8-d140-449b-ace8-4a8c2d3c5ada" />


## Características Principales

* **Arte ASCII Nativo:** Interfaces de usuario dibujadas completamente con caracteres en la terminal para menús, puntuaciones y pantallas de estado.
* **Gestión de Entradas (InputManager):** Captura de pulsaciones de teclado a bajo nivel para garantizar una respuesta inmediata y un control fluido de las paletas sin pausar la ejecución.
* **Arquitectura Modular:** Separación estricta entre la lógica del juego (`PongGame`), la física de las entidades (`Ball`, `Paddle`) y el dibujo en pantalla (`Renderer`).
* **Independencia Gráfica:** Construido puramente con bibliotecas estándar de C++ y llamadas al sistema, sin dependencias de terceros.

## Arquitectura y Estructura

```text
├── includes/
│   ├── Ball.h          # Física y lógica de colisión de la pelota
│   ├── InputManager.h  # Procesamiento y lectura de eventos del teclado
│   ├── Paddle.h        # Propiedades y movimiento de las paletas
│   ├── PongGame.h      # Lógica central, bucle principal y estados
│   └── Renderer.h      # Escritura y limpieza del búfer de la consola
├── src/
│   └── main.cpp        # Punto de entrada y orquestación de clases
└── README.md
```

## Requisitos Previos

*   Sistema Operativo: Windows (requerido para los binarios ejecutables `.exe` y la gestión nativa de la consola).
*   Compilador C++ (GCC/MinGW, Clang o MSVC).

## Compilación y Ejecución

Para compilar el código fuente en un archivo ejecutable, navega a la raíz del repositorio y ejecuta el compilador. Ejemplo utilizando `g++`:

```bash
# Compilar el código fuente
g++ src/*.cpp -I include -o pong.exe -O2

# Ejecutar el binario generado
./pong.exe
```

## Controles del Juego

**Movimiento:**
*   `W` o `^` (Flecha Arriba) - Mover hacia arriba
*   `S` o `v` (Flecha Abajo) - Mover hacia abajo

**Acciones Generales:**
*   `1` - Jugar / Reiniciar partida
*   `2` - Ver puntajes
*   `3` - Ver controles / Volver al menú / Pausa
*   `ESC` - Salir del juego
