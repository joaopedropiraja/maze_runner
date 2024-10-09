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

// Variáveis globais
Maze maze;
int num_rows;
int num_cols;
std::stack<Position> valid_positions;

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
        row < 0 || row > num_rows || 
        col < 0 || col > num_cols
    ) return false;

    return maze[row][col] == 'x' || maze[row][col] == 's';
}

// Função principal para navegar pelo labirinto
bool walk(Position pos) {
    // 4. Verifique se a posição atual é a saída (maze[pos.row][pos.col] == 's')
    if (maze[pos.row][pos.col] == 's') return true;

    // 1. Marque a posição atual como visitada (maze[pos.row][pos.col] = '.')
    maze[pos.row][pos.col] = '.';

    // 2. Chame print_maze() para mostrar o estado atual do labirinto
    print_maze();

    // 3. Adicione um pequeno atraso para visualização:
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    
    // 5. Verifique as posições adjacentes (cima, baixo, esquerda, direita)
    Position adjPos;
    adjPos.row = pos.row + 1;
    adjPos.col = pos.col;
    if (is_valid_position(adjPos.row, adjPos.col)) {
        if (walk(adjPos)) return true;
    }

    adjPos.row = pos.row - 1;
    if (is_valid_position(adjPos.row, adjPos.col)) {
        if (walk(adjPos)) return true;        
    }

    adjPos.row = pos.row;
    adjPos.col = pos.col + 1;
    if (is_valid_position(adjPos.row, adjPos.col)) {
        if (walk(adjPos)) return true;        
        
    }

    adjPos.col = pos.col - 1;
    if (is_valid_position(adjPos.row, adjPos.col)) {
        if (walk(adjPos)) return true;        
    }


    // 7. Se todas as posições foram exploradas sem encontrar a saída, retorne false    
    return false;
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

    bool exit_found = walk(initial_pos);

    if (exit_found) {
        std::cout << "Saída encontrada!" << std::endl;
    } else {
        std::cout << "Não foi possível encontrar a saída." << std::endl;
    }

    return 0;
}

// Nota sobre o uso de std::this_thread::sleep_for:
// 
// A função std::this_thread::sleep_for é parte da biblioteca <thread> do C++11 e posteriores.
// Ela permite que você pause a execução do thread atual por um período especificado.
// 
// Para usar std::this_thread::sleep_for, você precisa:
// 1. Incluir as bibliotecas <thread> e <chrono>
// 2. Usar o namespace std::chrono para as unidades de tempo
// 
// Exemplo de uso:
// std::this_thread::sleep_for(std::chrono::milliseconds(50));
// 
// Isso pausará a execução por 50 milissegundos.
// 
// Você pode ajustar o tempo de pausa conforme necessário para uma melhor visualização
// do processo de exploração do labirinto.