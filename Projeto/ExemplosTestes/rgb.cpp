#include <iostream>  // Inclui o arquivo de cabeçalho para entrada/saída padrão
#include "../External/glm/glm.hpp"  // Inclui o arquivo de cabeçalho para a biblioteca glm
#include "headers/calculator.h"  // Inclui o arquivo de cabeçalho para a classe calculator

int main() {
    calculator calculadora;  // Cria uma instância da classe calculator

    int soma = calculadora.sum(1, 2);  // Realiza uma soma usando a calculadora
    std::cout << soma << "\n";  // Imprime o resultado da soma

    return 0;  // Retorna 0 para indicar que o programa foi executado com sucesso
}

