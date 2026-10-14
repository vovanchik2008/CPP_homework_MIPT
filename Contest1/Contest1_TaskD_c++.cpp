#include <cstring>
#include <iostream>

int main() {
  // Динамическая матрица int с resize и print_type
  // 
  // При resize нам нужен указатель на матрицу, т.к. 
  // resize перевыделяет саму матрицу, а если бы мы передавали int** по значению, 
  // изменения остались бы в копии, matrix указывал бы на освобождённую память
  // 
  // В print_type мы сначала выбираем тип (void* позволяет хранить адрес, не зная типа заранее), 
  // а потом кастом приводим к нужному типу
  int rows = 1;
  int cols = 1;
  int** matrix = new int* [rows];
  for (int i = 0; i < rows; ++i) {
    matrix[i] = new int[cols];
  }
  matrix[0][0] = 2;
  int*** matrix_address = &matrix;
  char command[32];
  while (std::cin >> command) {
    if (std::strcmp(command, "break") == 0) {
      break;
    }
    if (std::strcmp(command, "resize") == 0) {
      int new_rows;
      int new_cols;
      std::cin >> new_rows >> new_cols;
      for (int i = 0; i < rows; ++i) {
        delete[](*matrix_address)[i];
      }
      delete[] *matrix_address;
      *matrix_address = new int* [new_rows];
      for (int i = 0; i < new_rows; ++i) {
        (*matrix_address)[i] = new int[new_cols];
      }
      rows = new_rows;
      cols = new_cols;
      for (int i = 0; i < new_rows; ++i) {
        for (int j = 0; j < new_cols; ++j) {
          matrix[i][j] = i + j;
        }
      }
    }
    if (std::strcmp(command, "print") == 0) {
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
      }
    }
    if (std::strcmp(command, "print_type") == 0) {
      void* first_line = matrix[0];
      char type_choose[16];
      std::cin >> type_choose;
      if (std::strcmp(type_choose, "int") == 0) {
        int* p = static_cast<int*>(first_line);
        for (int j = 0; j < cols; ++j) {
          std::cout << p[j] << " ";
        }
        std::cout << "\n";
      } else if (std::strcmp(type_choose, "char") == 0) {
        char* p = static_cast<char*>(first_line);
        for (int j = 0; j < cols * sizeof(int); ++j) {
          std::cout << static_cast<int>(p[j]) << " ";
        }
        std::cout << "\n";
      } else {
        std::cout << "Unknown type\n";
      }
    }
  }
  for (int i = 0; i < rows; ++i) {
    delete[](*matrix_address)[i];
  }
  delete[] * matrix_address;
  return 0;
}