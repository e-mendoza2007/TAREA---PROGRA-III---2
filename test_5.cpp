#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Conceptos

template <typename C>
concept Iterable = requires(C c) {
    begin(c);
    end(c);
};

template <typename T>
concept Addable = requires(T a, T b) {
    { a + b } -> same_as<T>;
};

template <typename T>
concept Divisible = requires(T a, size_t n) {
    { a / n } -> same_as<T>;
};

template <typename T, typename U>
concept Comparable = (
    (same_as<T, int> || same_as<T, double>) &&
    (same_as<U, int> || same_as<U, double>)
);

// Funciones base 

template <Iterable C>
requires Addable<typename C::value_type>
auto sum(const C& container) {
    using T = typename C::value_type;
    T result{};
    for (const auto& value : container)
        result = result + value;
    return result;
}

template <Divisible D>
D divi(D s, size_t n) {
    D d = s / n;
    return d;
}


template <typename Func, typename T>
concept Transformable = requires(Func f, T x) {
    { f(x) } -> convertible_to<double>;
};

template <Iterable C, typename Func>
requires requires(typename C::value_type x, Func f) {
    { f(x) } -> convertible_to<double>;
}
auto transform_reduce(const C& contenedor, Func func) {
    double resultado = 0.0;
    for (const auto& val : contenedor) {
        resultado = resultado + func(val);
    }
    return resultado;
}

// Clase para purebas

class Coordenada {
private:
    double x, y;
public:
    Coordenada(double x_ = 0, double y_ = 0) : x(x_), y(y_) {}
    
    double get_x() const { return x; }
    double get_y() const { return y; }
    
    operator double() const {
        return sqrt(x*x + y*y);
    }
};

// PRUEBAS 

int main() {
    cout << "\n==== test_transform_reduce.cpp ====\n";
    
    // ========== EJEMPLOS QUE COMPILAN ==========
    cout << "\n--- CASOS QUE COMPILAN ---\n";
    
    // 1. Tipo fundamental: double
    cout << "\n1. vector<double>:\n";
    vector<double> numeros = {1.5, 2.5, 3.5, 4.5};
    auto r1 = transform_reduce(numeros, [](double x) { return x * x; });
    cout << "   transform_reduce({1.5,2.5,3.5,4.5}, [](x){return x*x;}) = " << r1 << "\n";
    cout << "   (2.25 + 6.25 + 12.25 + 20.25 = 41.0)\n";
    cout << "   COMPILA: double es convertible a double\n";
    
    // 2. Tipo fundamental: int
    cout << "\n2. vector<int>:\n";
    vector<int> enteros = {1, 2, 3, 4};
    auto r2 = transform_reduce(enteros, [](int x) { return x * x; });
    cout << "   transform_reduce({1,2,3,4}, [](x){return x*x;}) = " << r2 << "\n";
    cout << "   (1 + 4 + 9 + 16 = 30)\n";
    cout << "   COMPILA: int es convertible a double\n";
    
    // 3. Objeto de clase: Coordenada
    cout << "\n3. vector<Coordenada>:\n";
    vector<Coordenada> coords = {Coordenada(3,4), Coordenada(1,2), Coordenada(5,12)};
    auto r3 = transform_reduce(coords, [](Coordenada c) { 
        return c.get_x() * c.get_y(); 
    });
    cout << "   transform_reduce(coordenadas, [](c){return c.x*c.y;}) = " << r3 << "\n";
    cout << "   (12 + 2 + 60 = 74)\n";
    cout << "   COMPILA: Coordenada se convierte a double\n";
    
    // ========== EJEMPLOS QUE NO COMPILAN ==========
    cout << "\n--- CASOS QUE NO COMPILAN ---\n";
    
    // 4. string (no convertible a double)
    cout << "\n4. vector<string> (NO COMPILA):\n";
    cout << "   // vector<string> palabras = {\"Hola\", \"Mundo\"};" << endl;
    cout << "   // auto r4 = transform_reduce(palabras, [](string s) { return s + s; });" << endl;
    cout << "   ERROR: string no es convertible a double\n";
    cout << "   CONCEPTO QUE FALLA: Transformable\n";
    
    // 5. char (no es directamente convertible)
    cout << "\n5. vector<char> (NO COMPILA):\n";
    cout << "   // vector<char> letras = {'a', 'b', 'c'};" << endl;
    cout << "   // auto r5 = transform_reduce(letras, [](char c) { return c; });" << endl;
    cout << "   ERROR: char no es directamente convertible a double\n";
    cout << "   CONCEPTO QUE FALLA: Transformable\n";
    
    return 0;
}