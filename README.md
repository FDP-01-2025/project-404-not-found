[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/mi1WNrHU)
# 👾 Proyecto de C++ - Centipede

![Centipede](resources/centipede.jpg)

## 📑 Descripción del Proyecto

**Temática y ambientación:**
El entorno podría decirse que representa un jardín en el que emerge un centipede de la parte superior y baja hacia el jugador.

**Mecánica principal:**
El juego consiste en disparar a la cabeza del centipede para ganar, moviendote usando las flechas de dirección para calcular tus tiros.

**Idea general de la jugabilidad:**


En arcade se usa trackball, lo que permite un movimiento suave y preciso, en cambio en el ordenador ocupas las flechas direccionales. 

Solo puedes tener un solo disparo en pantalla a la vez, lo que añade cierta dificultad.

Tienes 3 vidas, el centipede se mueve hacia abajo hasta llegar al shooter del jugador, para vencerlo tienes que disparale en la cabeza, de lo contario, perderás.

## ⚙ Aplicación de los temas vistos

**Variables y tipos de datos**: 

Se ocuparon variables del tipo `int` para las vidas y el puntaje.

Ejemplo:


`int score = 0;`

`int lives = 3;`

También tipo `char` para obtener la tecla presionada por el usuario.

Ejemplo:

`char tecla = 0;`

**Uso de If, else**:

Ejemplo:

Un ejemplo de la aplicación del if, else es en el menú del juego, en el que se le pregunta al usuario si quiere jugar o salir del juego.

`if (opcion == 1) {
            cout << "\nCARGANDO PARTIDA...\n";
            Sleep(1000);
            startGame();
}`

`else {
            cout << "\nGRACIAS POR JUGAR\n";
            break;
        }`

**Uso del bucle for**:

Ejemplo: 

Se aplicó este tipo de bucle para actualizar los disparos. A continuación se muestra un fragmento del código que cumple la función.

`    for (int i = 0; i < shots.size(); i++) 
{`

    `if (shots[i].active) {
            shots[i].y--;`

            if (shots[i].y < 0) {
                shots[i].active = false;
            }
        }
    }`

**Uso del bucle while**:

Ejemplo: 

Se utliza para controlar las funciones del juego cuando se está jugando.

`    while (!endGame()) {`

        clearScreenGame();
        drawScreenGame();
        showGame();
        updateGame();
        Sleep(10);
    }`

**Uso de Funciones**:

Ejemplo: Se usó una función tipo void para imprimir la pantalla del juego en la consola.

`void drawScreenGame() {`

    pantalla[HEIGHT - 1][playerX] = '^';

    for (auto& shot : shots) {
        if (shot.active && shot.y >= 0 && shot.y < HEIGHT) {
            pantalla[shot.y][shot.x] = '|';
        }
    }

    for (int cx : centipedeX) {
        if (centipedeY < HEIGHT) {
            pantalla[centipedeY][cx] = 'O';
        }
    }
}`

## Ejecución

## Consideraciones técnicas



## Mockups

![Pantalla de inicio](resources/mockup_pantalla_de_inicio.png)

![Pantalla de partida](resources/mockup_partida.png)


## 🕹 Equipo

- **Nombre del equipo:** Code_Warriors.cpp

### 👥Integrantes del equipo

1. **Nombre completo:** Milton Guillermo Rivas Palacios  
   **Carnet:** 00019725

2. **Nombre completo:** David Armando Romero Siliezer  
   **Carnet:** 00203725

3. **Nombre completo:** Diego Alejandro Tolentino  
   **Carnet:** 00128925

4. **Nombre completo:** Josué Miguel Peña Torres  
   **Carnet:** 00014425

## Instrucciones de Ejecución

1. Clona este repositorio en tu máquina local:
   ```bash
   git clone https://github.com/FDP-01-2025/project-404-not-found

