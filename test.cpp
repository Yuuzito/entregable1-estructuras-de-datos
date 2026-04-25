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
  std::cout << "--- INICIANDO TEST DE MOVING IMAGE ---\n\n";

  // 1. Instanciamos la imagen (esto llama al constructor)
  moving_image mi_imagen;
  std::cout << "Imagen creada. Guardando estado inicial...\n";
  mi_imagen.draw("paso_0_inicial.png");

  // 2. Hacemos algunos movimientos
  std::cout << "\n--- HACIENDO MOVIMIENTOS ---\n";
    
  std::cout << "Moviendo a la derecha 50 pixeles...\n";
  mi_imagen.move_right(50);
  mi_imagen.draw("paso_1_derecha.png");

  std::cout << "Moviendo hacia abajo 80 pixeles...\n";
  mi_imagen.move_down(80);
  mi_imagen.draw("paso_2_abajo.png");

  std::cout << "Rotando 90 grados...\n";
  mi_imagen.rotate();
  mi_imagen.draw("paso_3_rotado.png");

  // 3. Probamos el UNDO (Deshacer)
  std::cout << "\n--- PROBANDO UNDO (Viaje al pasado) ---\n";
    
  mi_imagen.undo(); // Debería deshacer la rotación
  mi_imagen.draw("paso_4_undo_rotacion.png");

  mi_imagen.undo(); // Debería deshacer el movimiento hacia abajo
  mi_imagen.draw("paso_5_undo_abajo.png");

  // 4. Probamos el REDO (Viaje al futuro)
  std::cout << "\n--- PROBANDO REDO (Viaje al futuro) ---\n";
    
  mi_imagen.redo(); // Debería rehacer el movimiento hacia abajo
  mi_imagen.draw("paso_6_redo_abajo.png");

  // 5. Probamos la bifurcación temporal (hacer algo nuevo tras un undo)
  std::cout << "\n--- PROBANDO NUEVA LINEA TEMPORAL ---\n";
    
  mi_imagen.undo(); // Volvemos a quitar el movimiento hacia abajo
  mi_imagen.draw("paso_7_undo_abajo.png");
  std::cout << "Se deshizo el movimiento hacia abajo.\n";
    
  std::cout << "Moviendo a la izquierda 30 pixeles (Crea nueva linea temporal)...\n";
  mi_imagen.move_left(30);
  mi_imagen.draw("paso_8_nueva_linea_izq.png");

  std::cout << "Intentando rehacer (Deberia fallar porque el futuro fue borrado)...\n";
  mi_imagen.redo(); // Aquí tu programa debería imprimir "No hay operaciones para rehacer"

  std::cout << "\n--- TEST FINALIZADO CORRECTAMENTE ---\n";
  std::cout << "Revisa tu carpeta para ver los archivos .png generados.\n";
  /* NOTA 1: Si usan el mismo nombre para las imágenes, entonces cada llamada al
  método draw() sobreescribirá a la imagen */

  /* NOTA 2: Si usan un ciclo while(1) y usan el mismo nombre de imagen,
  entonces se puede generar un efecto de desplazamiento continuo (algo así como
  un video =D ) */
  return 0;
}
