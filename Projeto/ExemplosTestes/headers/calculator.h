#ifndef CALCULATOR_H  
#define CALCULATOR_H

// Classe para representar uma calculadora simples
class calculator {
public:
    // Construtor
    calculator();

    // Métodos para operações matemáticas simples
    int sum(int x, int y);  // Soma
    int sub(int x, int y);  // Subtração
    int mult(int x, int y);  // Multiplicação
    float div(int x, int y);  // Divisão
};

#endif 
