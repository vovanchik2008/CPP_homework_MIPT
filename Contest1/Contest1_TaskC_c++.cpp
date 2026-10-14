#include <cstring>
#include <iostream>

void clear(char board[5][10]) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 10; ++j) {
      board[i][j] = ' ';
    }
  }
}

bool put(char board[5][10], int x, int y, char c) {
  if (y > 4 || y < 0 || x > 9 || x < 0) {
    return false;
  }
  board[y][x] = c;
  return true;
}

bool text(char board[5][10], int x, int y, char word[11]) {
  if (y > 4 || y < 0 || x > 9 || x < 0) {
    return false;
  }
  int length = 0;
  while (word[length] != '\0') {
    ++length;
  }
  if (x + length > 10) {
    return false;
  }
  for (int i = 0; i < length; ++i) {
    put(board, x + i, y, word[i]);
  }
  return true;
}

void print(const char board[5][10]) {
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 10; ++j) {
      std::cout << board[i][j];
    }
    std::cout << "\n";
  }
}

int main() {
  char board[5][10];
  clear(board);
  char command[32];
  while (std::cin >> command) {
    if (std::strcmp(command, "quit") == 0) {
      break;
    }
    if (std::strcmp(command, "clear") == 0) {
      clear(board);
    }
    else if (std::strcmp(command, "put") == 0) {
      int x;
      int y;
      char c;
      std::cin >> x >> y >> c;
      if (!put(board, x, y, c)) {
        std::cout << "Error\n";
      }
    }
    else if (std::strcmp(command, "text") == 0) {
      int x;
      int y;
      char word[11];
      std::cin >> x >> y >> word;
      if (!text(board, x, y, word)) {
        std::cout << "Error\n";
      }
    }
    else if (std::strcmp(command, "print") == 0) {
      print(board);
    }
  }
  return 0;
}