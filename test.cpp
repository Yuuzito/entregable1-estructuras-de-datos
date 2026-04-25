/* Compilación: g++ -g -o img test.cpp
 * Ejecución: ./img
 *
 * Luego de la ejecución se generarán 3 imagines: imagen.png,
 * imagen_desplazada_500.png e imagen_desplazada_600.png
 */

#include "moving_image.h"
#include<windows.h> // para sleep (linux). Usar  #include<windows.h> para Windows
#include<iostream>

int main() {
  moving_image img;

  // Estado inicial
  img.draw("output/00_inicial.png");

  // Movimientos básicos
  img.move_left(120);
  img.draw("output/01_move_left_120.png");

  img.move_right(40);
  img.draw("output/02_move_right_40.png");

  img.move_up(80);
  img.draw("output/03_move_up_80.png");

  img.move_down(30);
  img.draw("output/04_move_down_30.png");

  // Rotación
  img.rotate();
  img.draw("output/05_rotate.png");

  // Undo / Redo
  img.undo();
  img.draw("output/06_undo.png");

  img.redo();
  img.draw("output/07_redo.png");

  // Repeat (repite la última operación activa)
  img.repeat();
  img.draw("output/08_repeat.png");

  // Más operaciones para tener historial más completo
  img.move_left(60);
  img.draw("output/09_move_left_60.png");

  img.move_down(50);
  img.draw("output/10_move_down_50.png");

  img.undo();
  img.draw("output/11_undo.png");

  // Reproduce todo el historial desde el estado inicial
  // (además va guardando imágenes intermedias por cada paso)
  img.repeat_all();

  // Estado final luego de repeat_all
  
  std::cout << "Imagenes generadas en la carpeta output." << std::endl;

    /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
  método draw() sobreescribirá a la imagen */

  /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
  entonces se puede generar un efecto de desplazamiento continuo (algo así como
  un video =D ) */
  return 0;
}