#include <cstring>
#include <iostream>

void deposit(int& money, int k) {
  if (k > 0) {
    money += k;
    return;
  }
  std::cout << "Error\n";
}

void withdraw(int& money, int k) {
  if (k > 0 && money - k >= 0) {
    money -= k;
    return;
  }
  std::cout << "Error\n";
}

void balance(int& money, int k) {
  std::cout << money << "\n";
}

void reset(int& money, int k) {
  money = 0;
}

void dupe(int& money, int k) {
  money *= 2;
}

int main() {
  int money = 0;
  const int cNumCommands = 5;
  const char* names[cNumCommands] = { "deposit", "withdraw", "balance", "reset", "dupe" };
  void (*handlers[cNumCommands])(int&, int) = { deposit, withdraw, balance, reset, dupe };
  bool needs_k[cNumCommands] = { true, true, false, false, false };
  char command[32];
  while (true) {
    std::cin >> command;
    if (!std::cin) {
      break;
    }
    if (strcmp(command, "stop") == 0) {
      break;
    }
    int command_index = -1;
    for (int i = 0; i < cNumCommands; ++i) {
      if (std::strcmp(command, names[i]) == 0) {
        command_index = i;
        break;
      }
    }
    if (command_index == -1) {
      std::cout << "Error\n";
      continue;
    }
    int k = 0; 
    if (needs_k[command_index]) {
      std::cin >> k;
    }
    handlers[command_index](money, k);
  }
  return 0;
}