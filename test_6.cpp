#include <iostream>
#include <string>
#include <cmath>
using namespace std;

// Conceptos

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template <typename T, typename U>
concept Comparable = (
    (same_as<T, int> || same_as<T, double>) &&
    (same_as<U, int> || same_as<U, double>)
);


template<typename... Args>
auto suma_variadic(Args... args) {
    return (args + ...);
}

template<typename... Args>
requires (Addable<Args> && ...)
auto media_variadic(Args... args) {
    auto total = (args + ...);
    size_t cantidad = sizeof...(args);
    
    if constexpr (is_integral_v<decltype(total)>) {
        return static_cast<double>(total) / cantidad;
    } else {
        return total / cantidad;
    }
}

template<typename... Args>
requires (Comparable<Args, Args> && ...)
auto max_variadic(Args... args) {
    auto mayor = args;
    ((mayor = (args > mayor ? args : mayor)), ...);
    return mayor;
}

// Clase para pruebas:

class Punto2D {
private:
    double x, y;
public:
    Punto2D(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
    
    Punto2D operator+(const Punto2D& otro) const {
        return Punto2D(x + otro.x, y + otro.y);
    }
    
    bool operator>(const Punto2D& otro) const {
        return (x*x + y*y) > (otro.x*otro.x + otro.y*otro.y);
    }
    
    void imprimir() const {
        cout << "(" << x << ", " << y << ")";
    }
};

// Pruebas:

int main() {
    cout << "\n==== test_variadic.cpp ====\n";
    
    // Ejemplos que compilan
    cout << "\n--- CASOS QUE COMPILAN ---\n";
    
    // 1. Tipo fundamental: double
    cout << "\n1. suma_variadic con doubles:\n";
    auto s1 = suma_variadic(1.5, 2.5, 3.0, 4.0);
    cout << "   suma_variadic(1.5,2.5,3.0,4.0) = " << s1 << " (11.0)\n";
    cout << "   COMPILA: todos son double\n";
    
    cout << "\n2. media_variadic con doubles:\n";
    auto m1 = media_variadic(1.0, 2.0, 3.0, 4.0);
    cout << "   media_variadic(1.0,2.0,3.0,4.0) = " << m1 << " (2.5)\n";
    cout << "   COMPILA: todos son double\n";
    
    cout << "\n3. max_variadic con doubles:\n";
    auto max1 = max_variadic(1.5, 2.7, 0.3, 3.1);
    cout << "   max_variadic(1.5,2.7,0.3,3.1) = " << max1 << " (3.1)\n";
    cout << "   COMPILA: todos son double\n";
    
    // 2. Tipo fundamental: int
    cout << "\n4. suma_variadic con enteros:\n";
    auto s2 = suma_variadic(1, 2, 3, 4);
    cout << "   suma_variadic(1,2,3,4) = " << s2 << " (10)\n";
    cout << "   COMPILA: todos son int\n";
    
    cout << "\n5. media_variadic con enteros:\n";
    auto m2 = media_variadic(1, 2, 3, 4);
    cout << "   media_variadic(1,2,3,4) = " << m2 << " (2.5)\n";
    cout << "   COMPILA: enteros convertidos a double\n";
    
    cout << "\n6. max_variadic con enteros:\n";
    auto max2 = max_variadic(3, 7, 2, 9, 1);
    cout << "   max_variadic(3,7,2,9,1) = " << max2 << " (9)\n";
    cout << "   COMPILA: todos son int\n";
    
    // 3. Objeto de clase: Punto2D
    cout << "\n7. suma_variadic con Punto2D:\n";
    Punto2D p1(1, 2), p2(3, 4), p3(5, 6);
    auto s3 = suma_variadic(p1, p2, p3);
    cout << "   suma_variadic(p1,p2,p3) = ";
    s3.imprimir();
    cout << " (9,12)\n";
    cout << "   COMPILA: Punto2D tiene operator+\n";
    
    cout << "\n8. max_variadic con Punto2D:\n";
    Punto2D pa(1, 2), pb(3, 4), pc(0, 5);
    auto max3 = max_variadic(pa, pb, pc);
    cout << "   max_variadic(puntos) = ";
    max3.imprimir();
    cout << " (el de mayor distancia = 3,4)\n";
    cout << "   COMPILA: Punto2D tiene operator>\n";
    
    // Ejemplos que no compilan
    cout << "\n--- CASOS QUE NO COMPILAN ---\n";
    
    // 4. string con media_variadic
    cout << "\n9. media_variadic con strings (NO COMPILA):\n";
    cout << "   // auto err1 = media_variadic(string(\"a\"), string(\"b\"));\n";
    cout << "   ERROR: string no es Addable para media\n";
    cout << "   CONCEPTO QUE FALLA: Addable\n";
    
    // 5. Tipos mezclados
    cout << "\n10. suma_variadic con tipos mezclados (NO COMPILA):\n";
    cout << "   // auto err2 = suma_variadic(1, 2.5, 3);\n";
    cout << "   ERROR: tipos diferentes (int, double, int)\n";
    cout << "   CONCEPTO QUE FALLA: No hay operator+ entre tipos distintos\n";
    
    // 6. max_variadic con strings
    cout << "\n11. max_variadic con strings (NO COMPILA):\n";
    cout << "   // auto err3 = max_variadic(\"hola\", \"mundo\");\n";
    cout << "   ERROR: string no tiene operator> y no es int/double\n";
    cout << "   CONCEPTO QUE FALLA: Comparable\n";
    
    return 0;
}