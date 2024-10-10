#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <stack>
#include <thread>
#include <chrono>

// Representação do labirinto
using Maze = std::vector<std::vector<char>>;

// Estrutura para representar uma posição no labirinto
struct Position {
    int row;
    int col;
};

void walk(Position pos);

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
int num_threads = 0;
std::stack<Position> valid_positions;
bool exit_found = false;


// Função para carregar o labirinto de um arquivo
Position load_maze(const std::string& file_name) {
    // TODO: Implemente esta função seguindo estes passos:
    // 1. Abra o arquivo especificado por file_name usando std::ifstream
    std::ifstream file(file_name);
    if (!file) {
        std::cout << "Error opening file: " << file_name << "." << std::endl;
        exit(EXIT_FAILURE);
    }

    // 2. Leia o número de linhas e colunas do labirinto
    file >> num_rows >> num_cols;
    file.ignore();

    // 3. Redimensione a matriz 'maze' de acordo (use maze.resize())
    maze.resize(num_rows, std::vector<char>(num_cols));

    // 4. Leia o conteúdo do labirinto do arquivo, caractere por caractere
    // 5. Encontre e retorne a posição inicial ('e')
    Position initial_pos;
    for (int i = 0; i < num_rows; i++) {
        std::string line;
        std::getline(file, line);

        for (int j = 0; j < num_cols; j++) {
            if ((maze[i][j] = line[j]) == 'e') {
                initial_pos = { .row = i, .col = j };
            }
        }
    }

    // 6. Trate possíveis erros (arquivo não encontrado, formato inválido, etc.)
    // 7. Feche o arquivo após a leitura
    file.close();

    return initial_pos;
}

// Função para imprimir o labirinto
void print_maze() {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            std::cout << maze[i][j];
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;
}

// Função para verificar se uma posição é válida
bool is_valid_position(int row, int col) {
    if (
        row < 0 || row >= num_rows || 
        col < 0 || col >= num_cols
    ) return false;

    return maze[row][col] == 'x' || maze[row][col] == 's';
}

void validatePos(int row, int col, Position &nextPos, bool &foundNextPos) {
  if (!is_valid_position(row, col)) return;

  if (foundNextPos) {
    num_threads++;

    std::thread t(walk, (Position){ row, col });
    t.detach();
  } else {
    foundNextPos = true;
    nextPos = (Position){ row, col };
  }
};

// Função principal para navegar pelo labirinto
void walk(Position pos) {
  if (maze[pos.row][pos.col] == 's') {
    exit_found = true;
    num_threads--;

    return;    
  };

  maze[pos.row][pos.col] = '.';
  std::this_thread::sleep_for(std::chrono::milliseconds(50));

  bool foundNextPos = false;
  Position nextPos;
  
  // Direita
  validatePos(pos.row, pos.col + 1, nextPos, foundNextPos);
  // Esquerda
  validatePos(pos.row, pos.col - 1, nextPos, foundNextPos); 
  // Em cima
  validatePos(pos.row + 1, pos.col, nextPos, foundNextPos); 
  // Embaixo
  validatePos(pos.row - 1, pos.col, nextPos, foundNextPos);     

  if (foundNextPos) {
    walk(nextPos);
    return;
  }

  num_threads--;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_labirinto>" << std::endl;
        return 1;
    }

    Position initial_pos = load_maze(argv[1]);
    if (initial_pos.row == -1 || initial_pos.col == -1) {
        std::cerr << "Posição inicial não encontrada no labirinto." << std::endl;
        return 1;
    }

    num_threads++;

    std::thread startThread(walk, initial_pos);
    startThread.detach();

    while (!exit_found && num_threads > 0) {
      print_maze();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}
