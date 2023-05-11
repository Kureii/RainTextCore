
bool returnTrue() { return true; }

/*#include <iostream>
#include <vector>

const int SIZE = 3;

int get_index(int layer, int row, int col) {
  return layer * SIZE * SIZE + row * SIZE + col;
}

int main() {
  std::vector<int> cube(SIZE * SIZE * SIZE, 0);
  int color = 1;

  for (int layer = 0; layer < SIZE; ++layer) {
    for (int row = 0; row < SIZE; ++row) {
      for (int col = 0; col < SIZE; ++col) {
        cube[get_index(layer, row, col)] = color++;
      }
    }
  }

  for (int layer = 0; layer < SIZE; ++layer) {
    for (int row = 0; row < SIZE; ++row) {
      for (int col = 0; col < SIZE; ++col) {
        std::cout << cube[get_index(layer, row, col)] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}*/


#include <array>
#include <iostream>

constexpr int DIM1 = 4;
constexpr int DIM2 = 4;
constexpr int DIM3 = 4;
constexpr int DIM4 = 4;



int main() {
  std::array<
      std::array<std::array<std::array<unsigned char, DIM4>, DIM3>, DIM2>, DIM1>
      byteAlphabet4D{};

  int value = 0x00;
  for (int i = 0; i < DIM1; i++) {
    for (int j = 0; j < DIM2; j++) {
      for (int k = 0; k < DIM3; k++) {
        for (int l = 0; l < DIM4; l++) {
          byteAlphabet4D[i][j][k][l] = value;
          value++;
          if (value > 0xFF) {
            value = 0x00;
          }
        }
      }
    }
  }

  // Výpis 4D pole
  for (int i = 0; i < DIM1; i++) {
    std::cout << "[";
    for (int j = 0; j < DIM2; j++) {
      std::cout << "[";
      for (int k = 0; k < DIM3; k++) {
        std::cout << "[";
        for (int l = 0; l < DIM4; l++) {
          std::cout << std::hex << (int)byteAlphabet4D[i][j][k][l] << ", ";
        }
        std::cout << "\b\b], " << std::endl;
      }
      std::cout << "\b\b], "  << std::endl;
    }
    std::cout << "\b\b]"  << std::endl;
  }

  for (int i = 0; i < DIM1; i++) {
    for (int j = 0; j < DIM2; j++) {
      for (int k = 0; k < DIM3; k++) {
        for (int l = 0; l < DIM4; l++) {
          std::cout << "["<< i << "][" << j << "][" << k << "][" << l << "]" << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  return 0;
}
