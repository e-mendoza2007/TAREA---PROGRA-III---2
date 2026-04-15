#include <iostream>
#include <vector>
#include <string>
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

// Funcionas base:

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


template<typename C>
requires Iterable<C> && Addable<typename C::value_type>
auto media_con_if(const C& contenedor) {
    using T = typename C::value_type;
    auto total = sum(contenedor);
    size_t n = 0;
    for (const auto& _ : contenedor) n++;
    
    if constexpr (is_integral_v<T>) {
        return static_cast<double>(total) / n;
    } else {
        return divi(total, n);
    }
}

template<typename T>
T procesar_numero(T valor) {
    if constexpr (is_integral_v<T>) {
        cout << "Entero: ";
        return valor * 2;
    } else if constexpr (is_floating_point_v<T>) {
        cout << "Flotante: ";
        return valor * 2.5;
    } else {
        cout << "Otro tipo: ";
        return valor;
    }
}

// Clase para pruebas:

class Dinero {
private:
    double cantidad;
public:
    Dinero(double c = 0) : cantidad(c) {}
    
    Dinero operator+(const Dinero& otro) const {
        return Dinero(cantidad + otro.cantidad);
    }
    
    Dinero operator/(size_t n) const {
        return Dinero(cantidad / n);
    }
    
    double get_cantidad() const { return cantidad; }
};

// Pruebas:

int main() {
    cout << "\n==== test_if_constexpr.cpp ====\n";
    
    // Ejemplos que compilan
    cout << "\n--- CASOS QUE COMPILAN ---\n";
    
    // 1. Tipo fundamental: int (entero)
    cout << "\n1. vector<int> (tipo entero):\n";
    vector<int> enteros = {1, 2, 3, 4};
    auto m1 = media_con_if(enteros);
    cout << "   media_con_if({1,2,3,4}) = " << m1 << " (2.5)\n";
    cout << "   COMPILA: if constexpr usa rama is_integral, convierte a double\n";
    
    // 2. Tipo fundamental: double (flotante)
    cout << "\n2. vector<double> (tipo flotante):\n";
    vector<double> flotantes = {1.0, 2.0, 3.0, 4.0};
    auto m2 = media_con_if(flotantes);
    cout << "   media_con_if({1.0,2.0,3.0,4.0}) = " << m2 << " (2.5)\n";
    cout << "   COMPILA: if constexpr usa rama else, division directa\n";
    
    // 3. Objeto de clase: Dinero
    cout << "\n3. vector<Dinero> (objeto de clase):\n";
    vector<Dinero> dineroes = {Dinero(10.5), Dinero(20.0), Dinero(15.5)};
    auto m3 = media_con_if(dineroes);
    cout << "   media_con_if({10.5,20.0,15.5}) = " << m3.get_cantidad() << "\n";
    cout << "   COMPILA: Dinero es Addable y Divisible, usa rama else\n";
    
    // 4. procesar_numero con int
    cout << "\n4. procesar_numero con int:\n";
    cout << "   ";
    auto r1 = procesar_numero(5);
    cout << r1 << " (10)\n";
    cout << "   COMPILA: if constexpr elige rama is_integral\n";
    
    // 5. procesar_numero con double
    cout << "\n5. procesar_numero con double:\n";
    cout << "   ";
    auto r2 = procesar_numero(3.14);
    cout << r2 << " (7.85)\n";
    cout << "   COMPILA: if constexpr elige rama is_floating_point\n";
    
    // Ejemplos que no compilan
    cout << "\n--- CASOS QUE NO COMPILAN ---\n";
    
    // 6. vector<string>
    cout << "\n6. vector<string> (NO COMPILA):\n";
    cout << "   // vector<string> textos = {\"a\", \"b\", \"c\"};" << endl;
    cout << "   // auto err1 = media_con_if(textos);" << endl;
    cout << "   ERROR: string no es Addable\n";
    cout << "   CONCEPTO QUE FALLA: Addable\n";
    
    // 7. int no iterable
    cout << "\n7. int (NO COMPILA):\n";
    cout << "   // int x = 5;" << endl;
    cout << "   // auto err2 = media_con_if(x);" << endl;
    cout << "   ERROR: int no es Iterable\n";
    cout << "   CONCEPTO QUE FALLA: Iterable\n";
    
    // 8. vector<char>
    cout << "\n8. vector<char> (NO COMPILA):\n";
    cout << "   // vector<char> letras = {'a', 'b', 'c'};" << endl;
    cout << "   // auto err3 = media_con_if(letras);" << endl;
    cout << "   ERROR: char sumado da int, no char\n";
    cout << "   CONCEPTO QUE FALLA: Addable (a+b debe dar T)\n";
    
    return 0;
}