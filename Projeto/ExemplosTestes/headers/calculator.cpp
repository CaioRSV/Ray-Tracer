#include "calculator.h"  

// Construtor da classe calculator
calculator::calculator() {}

// Método para somar dois números inteiros
int calculator::sum(int x, int y) {
    return x + y;
}

// Método para subtrair dois números inteiros
int calculator::sub(int x, int y) {
    return x - y;
}

// Método para multiplicar dois números inteiros
int calculator::mult(int x, int y) {
    return x * y;
}

// Método para dividir dois números inteiros e retornar um valor float
float calculator::div(int x, int y) {
    return static_cast<float>(x) / y;
}

