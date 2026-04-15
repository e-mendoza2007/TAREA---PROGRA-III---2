#include<iostream>
#include<vector>
#include <cmath>
using namespace std;


template < typename C >
concept Iterable = requires ( C c ) {
    begin ( c ) ;
     end ( c ) ;
};
template < typename T >
concept Addable = requires ( T a , T b ) {
    { a + b } -> same_as <T >;
};
template < typename T >
concept Divisible = requires ( T a , size_t n ) {
    { a / n } -> same_as <T >;
};
// añadimos un nuevo concep para poder verificar que dos numeros se pueden comparar, no string no char
// aunque char tmb se puede, pero nos restringimos a solo double e int
template < typename T,typename U >
concept Comparable = (
    (same_as< T,int > || same_as< T, double >) &&
    (same_as< U, int > || same_as< U, double >)
);

// ####################### Pregunta 2 #####################33
template < Iterable C >
requires Addable < typename C :: value_type >
auto sum ( const C & container )
{
    using T = typename C :: value_type ;
    T result {};
    for ( const auto & value : container )
        result = result + value ;
    return result ;
}

template < Divisible D>
D divi( D s , size_t n) {

    D d = s / n;
    return d;
}

template < typename C >
requires Addable<typename C::value_type> &&
         Divisible<typename C::value_type>
auto mean(C contenedor) {

     auto suma = sum(contenedor);
    auto promedio =  divi(suma, contenedor.size());
return promedio;
}

// ############################### Pregunta 3 ##############

template < Iterable C >
requires Addable < typename C :: value_type >
auto varianza ( const C & container,auto promedio_ ) {

    using T = typename C :: value_type ;
    T rpt{};
    for ( const auto & value : container ) {
        auto resta = value - promedio_ ;
       rpt = rpt + pow(resta,2);
    };
    return rpt;
}


template < typename T >
requires Addable<typename T::value_type> &&
         Divisible<typename T::value_type>
auto variance( T conte ) {

    auto promedio = mean(conte);
    auto suma_el = varianza(conte,promedio);
    auto varianza_respuesta = divi(suma_el,conte.size());
    return round(varianza_respuesta*100)/100;
};

// ##################################### Pregunnta 4 ###########################
template < Iterable C >
requires Comparable < typename C :: value_type , typename C :: value_type >
auto maximo ( const C & container ) {

    auto maximo = 0.0;
    for ( const auto & value : container ) {
        if (value > maximo) {
            maximo = value;
        }
    }
    return maximo;
}


template < typename M >
requires Comparable<typename M::value_type, typename M::value_type>
auto max( M conte ) {

    auto maximo_ = maximo(conte);

    return maximo_;
}


// Imprimir vector
template <typename T>
void imprimir(const vector<T>& v) {
    for (const auto& x : v) {
        cout << x << " ";
    }
    cout << endl;
}

// Concepto propio: Transformable (para transform_reduce)
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

// ==================== PARTE 6: Variadic templates ====================

// suma_variadic
template<typename... Args>
auto suma_variadic(Args... args) {
    return (args + ...);
}

// media_variadic
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

// max_variadic (usando el Comparable de tu compañero)
template<typename... Args>
requires (Comparable<Args, Args> && ...)
auto max_variadic(Args... args) {
    auto mayor = args;
    ((mayor = (args > mayor ? args : mayor)), ...);
    return mayor;
}

// ==================== PARTE 7: if constexpr ====================

// Media con if constexpr (versión mejorada)
template<typename C>
requires Iterable<C> && Addable<typename C::value_type>
auto media_con_if(const C& contenedor) {
    using T = typename C::value_type;
    auto total = sum(contenedor);
    size_t n = 0;
    for (const auto& _ : contenedor) n++;
    
    if constexpr (is_integral_v<T>) {
        // Si es entero, convertir a double
        return static_cast<double>(total) / n;
    } else {
        // Si no es entero, usar divi
        return divi(total, n);
    }
}

// Función adicional para demostrar if constexpr
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

int main() {

    cout << "################## Pregunta 2 ################\n";
    vector<double> a{1.0, 2.0, 3.0};
    cout << "Vector: " << endl;
    imprimir(a);
    auto m = mean(a);
    cout << "Promedio: " << m << endl;
    
    cout << "################## Pregunta 3 ################\n";
    vector<double> data{1.0, 2.0, 3.0};
    cout << "Vector: " << endl;
    imprimir(data);
    auto v = variance(data);
    cout << "Varianza: " << v << endl;
    
    cout << "################## Pregunta 4 ################\n";
    vector<double> b{1, 2.7, 0.3};
    cout << "Vector: " << endl;
    imprimir(b);
    auto maximo_ = maximo(b);
    cout << "Maximo: " << maximo_ << endl;
    
    // Parte 5
    cout << "\n Parte 5: transform_reduce \n";
    
    vector<double> numeros = {1.5, 2.5, 3.5, 4.5};
    auto r1 = transform_reduce(numeros, [](double x) { return x * x; });
    cout << "transform_reduce(cuadrados) = " << r1 << " (2.25+6.25+12.25+20.25=41)\n";
    
    vector<int> enteros = {1, 2, 3, 4};
    auto r2 = transform_reduce(enteros, [](int x) { return x * x; });
    cout << "transform_reduce(enteros al cuadrado) = " << r2 << " (1+4+9+16=30)\n";
    
    // Parte 6
    cout << "\n Parte 6: Variadic templates \n";
    
    // suma_variadic
    auto s1 = suma_variadic(1, 2, 33, 4);
    cout << "suma_variadic(1,2,33,4) = " << s1 << " (40)\n";
    
    auto s2 = suma_variadic(1.5, 2.5, 3.0);
    cout << "suma_variadic(1.5,2.5,3.0) = " << s2 << " (7.0)\n";
    
    // media_variadic
    auto m1 = media_variadic(1, 2, 3, 4);
    cout << "media_variadic(1,2,3,4) = " << m1 << " (2.5)\n";
    
    auto m2 = media_variadic(1.0, 2.0, 3.0, 4.0);
    cout << "media_variadic(1.0,2.0,3.0,4.0) = " << m2 << " (2.5)\n";
    
    // max_variadic
    auto max1 = max_variadic(3, 7, 2, 9, 1, 5);
    cout << "max_variadic(3,7,2,9,1,5) = " << max1 << " (9)\n";
    
    auto max2 = max_variadic(1.5, 2.7, 0.3, 3.1);
    cout << "max_variadic(1.5,2.7,0.3,3.1) = " << max2 << " (3.1)\n";
    
    // Parte 7
    cout << "\n Parte 7: if constexpr \n";
    
    // Media con if constexpr
    vector<int> enteros2 = {1, 2, 3, 4};
    auto mif1 = media_con_if(enteros2);
    cout << "media_con_if({1,2,3,4}) con enteros = " << mif1 << " (convierte a double)\n";
    
    vector<double> flotantes = {1.0, 2.0, 3.0, 4.0};
    auto mif2 = media_con_if(flotantes);
    cout << "media_con_if({1,2,3,4}) con doubles = " << mif2 << " (division directa)\n";
    
    // procesar_numero
    cout << "\nprocesar_numero(5) = ";
    cout << procesar_numero(5) << endl;
    
    cout << "procesar_numero(3.14) = ";
    cout << procesar_numero(3.14) << endl;
    
    cout << "procesar_numero(2.5f) = ";
    cout << procesar_numero(2.5f) << endl;
    
    return 0;
}