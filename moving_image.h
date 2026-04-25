#ifndef MOVING_IMG_H
#define MOVING_IMG_H

#include "basics.h"
#include <stack>
#include <queue>
#include <stdexcept>
#include <string>
// Clase que representa una imagen como una colección de 3 matrices siguiendo el
// esquema de colores RGB

class moving_image {
private:
  unsigned char **red_layer; // Capa de tonalidades rojas
  unsigned char **green_layer; // Capa de tonalidades verdes
  unsigned char **blue_layer; // Capa de tonalidades azules
  std::stack<int> stack_undo; // Pila para almacenar las operaciones realizadas sobre la imagen, con el fin de poder deshacerlas
  std::stack<int> stack_redo; // Pila para almacenar las operaciones deshechas sobre la imagen, con el fin de poder rehacerlas
  std::queue<int> queue_all;

  void _reset_to_initial_state() {
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++) {
        red_layer[i][j] = DEFAULT_R;
        green_layer[i][j] = DEFAULT_G;
        blue_layer[i][j] = DEFAULT_B;
      }

    for(int i=0; i < 322; i++)
      for(int j=0; j < 256; j++) {
        if(!s_R[i][j] && !s_G[i][j] && !s_B[i][j]) {
          red_layer[INIT_Y+i][INIT_X+j] = DEFAULT_R;
          green_layer[INIT_Y+i][INIT_X+j] = DEFAULT_G;
          blue_layer[INIT_Y+i][INIT_X+j] = DEFAULT_B;
        } else {
          red_layer[INIT_Y+i][INIT_X+j] = s_R[i][j];
          green_layer[INIT_Y+i][INIT_X+j] = s_G[i][j];
          blue_layer[INIT_Y+i][INIT_X+j] = s_B[i][j];
        }
      }
  }
public:
  // Constructor de la imagen. Se crea una imagen por defecto
  moving_image() {
    // Reserva de memoria para las 3 matrices RGB
    red_layer = new unsigned char*[H_IMG];
    green_layer = new unsigned char*[H_IMG];
    blue_layer = new unsigned char*[H_IMG];
    
    for(int i=0; i < H_IMG; i++) {
      red_layer[i] = new unsigned char[W_IMG];
      green_layer[i] = new unsigned char[W_IMG];
      blue_layer[i] = new unsigned char[W_IMG];
    }

    _reset_to_initial_state();
  }

  // Destructor de la clase
  ~moving_image() {
    for(int i=0; i < H_IMG; i++) {
      delete red_layer[i];
      delete green_layer[i];
      delete blue_layer[i];
    }

    delete red_layer;
    delete green_layer;
    delete blue_layer;
  }

  // Función utilizada para guardar la imagen en formato .png
  void draw(const char* nb) {
    _draw(nb);
  }
  void validarNoNegativo(int d) {
    if (d < 0) {
      throw std::invalid_argument("La distancia no puede ser negativa");
    }
  }
  void move_left(int d) {
    validarNoNegativo(d);
    while(!stack_redo.empty()) stack_redo.pop();
    stack_undo.push(1);
    stack_undo.push(d);
    queue_all.push(d);
    queue_all.push(1);
    
    _move_left_internal(d);
  }

private:
  void _move_left_internal(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	      tmp_layer[i][j] = red_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][j] = red_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	  tmp_layer[i][j] = green_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][j] = green_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  green_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	  tmp_layer[i][j] = blue_layer[i][j+d];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][j] = blue_layer[i][k];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  blue_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++) delete[] tmp_layer[i];
    delete[] tmp_layer;
  }

public:
  void move_right(int d) {
    validarNoNegativo(d);
    while(!stack_redo.empty()) stack_redo.pop();
    stack_undo.push(2);
    stack_undo.push(d);
    queue_all.push(d);
    queue_all.push(2);
    _move_right_internal(d);
  }

private:
  void _move_right_internal(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
	      tmp_layer[i][j+d] = red_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][k] = red_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	  tmp_layer[i][j+d] = green_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][k] = green_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  green_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG-d; j++)
    	  tmp_layer[i][j+d] = blue_layer[i][j];      
    
    for(int i=0; i < H_IMG; i++)
      for(int j=W_IMG-d, k=0; j < W_IMG; j++, k++)
    	  tmp_layer[i][k] = blue_layer[i][j];      

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  blue_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++) delete[] tmp_layer[i];
    delete[] tmp_layer;
  }

public:

  void move_down(int d) {
    validarNoNegativo(d);
    while(!stack_redo.empty()) stack_redo.pop();
    stack_undo.push(3);
    stack_undo.push(d);
    queue_all.push(d);
    queue_all.push(3);
    _move_down_internal(d);
  }

private:
  void _move_down_internal(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    for(int i = 0; i < H_IMG - d; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[i+d][j] = red_layer[i][j];

    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[k][j] = red_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    for(int i = 0; i < H_IMG - d; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[i+d][j] = green_layer[i][j];

    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[k][j] = green_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    for(int i = 0; i < H_IMG - d; i++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[i+d][j] = blue_layer[i][j];

    for(int i = H_IMG - d, k = 0; i < H_IMG; i++, k++)
      for(int j = 0; j < W_IMG; j++)
        tmp_layer[k][j] = blue_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++) delete[] tmp_layer[i];
    delete[] tmp_layer;
  }

public:

  void move_up(int d) {
    validarNoNegativo(d);
    while(!stack_redo.empty()) stack_redo.pop();
    stack_undo.push(4);
    stack_undo.push(d);
    queue_all.push(d);
    queue_all.push(4);
    _move_up_internal(d);
  }

private:
  void _move_up_internal(int d) {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];
    
    for(int j=0; j < W_IMG; j++)
      for(int i=d; i < H_IMG; i++)
	      tmp_layer[i-d][j] = red_layer[i][j];      

    for(int j=0; j < W_IMG; j++)
      for(int i=H_IMG-d, k=0; i < H_IMG; i++, k++)
    	  tmp_layer[i][j] = red_layer[k][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
	      red_layer[i][j] = tmp_layer[i][j];

    for(int j=0; j < W_IMG; j++)
      for(int i=d; i < H_IMG; i++)
	      tmp_layer[i-d][j] = green_layer[i][j];      

    for(int j=0; j < W_IMG; j++)
      for(int i=H_IMG-d, k=0; i < H_IMG; i++, k++)
    	  tmp_layer[i][j] = green_layer[k][j];    

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  green_layer[i][j] = tmp_layer[i][j];

    for(int j=0; j < W_IMG; j++)
      for(int i=d; i < H_IMG; i++)
	      tmp_layer[i-d][j] = blue_layer[i][j];      

    for(int j=0; j < W_IMG; j++)
      for(int i=H_IMG-d, k=0; i < H_IMG; i++, k++)
    	  tmp_layer[i][j] = blue_layer[k][j];     

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
    	  blue_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++) delete[] tmp_layer[i];
    delete[] tmp_layer;
  }

public:
  void rotate() {
    while(!stack_redo.empty()) stack_redo.pop();
    stack_undo.push(5);
    stack_undo.push(90);
    queue_all.push(90);
    queue_all.push(5);
    _rotate_internal();
  }

private:
  void _rotate_internal() {
    unsigned char **tmp_layer = new unsigned char*[H_IMG];
    for(int i=0; i < H_IMG; i++) 
      tmp_layer[i] = new unsigned char[W_IMG];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        tmp_layer[H_IMG - j - 1][i] = red_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        red_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        tmp_layer[H_IMG - j - 1][i] = green_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        green_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        tmp_layer[H_IMG - j - 1][i] = blue_layer[i][j];

    for(int i=0; i < H_IMG; i++)
      for(int j=0; j < W_IMG; j++)
        blue_layer[i][j] = tmp_layer[i][j];

    for(int i=0; i < H_IMG; i++) delete[] tmp_layer[i];
    delete[] tmp_layer;
  }

public:

void undo() {
  if(stack_undo.empty()) {
    printf("No hay operaciones para deshacer\n");
    return;
  }

  int d = stack_undo.top();
  stack_undo.pop();
  int op = stack_undo.top();
  stack_undo.pop();

  stack_redo.push(op);
  stack_redo.push(d);

  switch(op) {
    case 1:
      queue_all.push(d);
      queue_all.push(-1);
      _move_right_internal(d);
      break;
    case 2:
      queue_all.push(d);
      queue_all.push(-2);
      _move_left_internal(d);
      break;
    case 3:
      queue_all.push(d);
      queue_all.push(-3);
      _move_up_internal(d);
      break;
    case 4:
      queue_all.push(d);
      queue_all.push(-4);
      _move_down_internal(d);
      break;
    case 5:
      queue_all.push(d);
      queue_all.push(-5);
      _rotate_internal();
      _rotate_internal();
      _rotate_internal();
      break;
    default:
      break;
  }
}

void redo() {
  if(stack_redo.empty()) {
    printf("No hay operaciones para rehacer\n");
    return;
  }

  int d = stack_redo.top();
  stack_redo.pop();
  int op = stack_redo.top();
  stack_redo.pop();

  stack_undo.push(op);
  stack_undo.push(d);

  switch(op) {
    case 1:
      queue_all.push(d);
      queue_all.push(0);
      _move_left_internal(d);
      break;
    case 2:
      queue_all.push(d);
      queue_all.push(6);
      _move_right_internal(d);
      break;
    case 3:
      queue_all.push(d);
      queue_all.push(7);
      _move_down_internal(d);
      break;
    case 4:
      queue_all.push(d);
      queue_all.push(8);
      _move_up_internal(d);
      break;
    case 5:
      queue_all.push(d);
      queue_all.push(9);
      _rotate_internal();
      break;
    default:
      break;
  }
}

void repeat() {
  if(stack_undo.empty()) {
    printf("No hay operaciones para repetir\n");
    return;
  }
    
  // Obtener la última operación
  int d = stack_undo.top(); 
  stack_undo.pop();
  int op = stack_undo.top();
  stack_undo.pop();

  // Restaurar la original
  stack_undo.push(op);
  stack_undo.push(d);
  queue_all.push(d);
  queue_all.push(op);
  // Limpiar redo (esto SÍ debe ir antes de registrar nueva acción)
  while(!stack_redo.empty()) stack_redo.pop();

  // Registrar la repetición como nueva operación
  stack_undo.push(op);
  stack_undo.push(d);
  switch(op) {
    case 1:
      _move_left_internal(d);
      break;
    case 2:
      _move_right_internal(d);
      break;
    case 3:
      _move_down_internal(d);
      break;
    case 4:
      _move_up_internal(d);
      break;
    case 5:
      _rotate_internal();
      break;
    default:
      break;
  }
}

void repeat_all() {
  if(queue_all.empty()) {
    printf("No hay operaciones para reproducir\n");
    return;
  }

  // La reproducción siempre arranca desde la imagen original.
  _reset_to_initial_state();

  int size = queue_all.size();
  for(int i = 1; i < size / 2 + 1; i++){

    std::string nombre = "output/(Repeat_all)Paso_" + std::to_string(i) + "_";
    int d = queue_all.front();
    queue_all.push(d);
    queue_all.pop();
    int op = queue_all.front();
    queue_all.push(op);
    queue_all.pop();
    std::string nombre_mov = "";
    switch(op) {
      
      case 1:
        nombre_mov = "move_left";
        _move_left_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 2:
        nombre_mov = "move_right";
        _move_right_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 3:
        nombre_mov = "move_down";
        _move_down_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 4:
        nombre_mov = "move_up";
        _move_up_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 5:
        nombre_mov = "rotate";
        _rotate_internal();
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case -1:
        nombre_mov = "undo_move_left";
        _move_right_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case -2:
        nombre_mov = "undo_move_right";
        _move_left_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case -3:
        nombre_mov = "undo_move_down";
        _move_up_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case -4:
        nombre_mov = "undo_move_up";
        _move_down_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case -5:
        nombre_mov = "undo_rotate";
        _rotate_internal();
        _rotate_internal();
        _rotate_internal();
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 0:
        nombre_mov = "redo_move_left";
        _move_left_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 6:
        nombre_mov = "redo_move_right";
        _move_right_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 7:
        nombre_mov = "redo_move_down";
        _move_down_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 8:
        nombre_mov = "redo_move_up";
        _move_up_internal(d);
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      case 9:
        nombre_mov = "redo_rotate";
        _rotate_internal();
        draw((nombre + nombre_mov + ".png").c_str());
        break;
      default:
        break;
    }
  }
  

}
private:
  // Función privada que guarda la imagen en formato .png
  void _draw(const char* nb) {
    //    unsigned char rgb[H_IMG * W_IMG * 3], *p = rgb;
    unsigned char *rgb = new unsigned char[H_IMG * W_IMG * 3];
    unsigned char *p = rgb;
    unsigned x, y;

    // La imagen resultante tendrá el nombre dado por la variable 'nb'
    FILE *fp = fopen(nb, "wb");

    // Transformamos las 3 matrices en una arreglo unidimensional
    for (y = 0; y < H_IMG; y++)
        for (x = 0; x < W_IMG; x++) {
            *p++ = red_layer[y][x];    /* R */
            *p++ = green_layer[y][x];    /* G */
            *p++ = blue_layer[y][x];    /* B */
        }
    // La función svpng() transforma las 3 matrices RGB en una imagen PNG 
    svpng(fp, W_IMG, H_IMG, rgb, 0);
    fclose(fp);
    delete[] rgb;
}

  
};

#endif
