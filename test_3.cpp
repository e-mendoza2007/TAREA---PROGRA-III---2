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
auto variance( T conte ) {

    auto promedio = mean(conte);
    auto suma_el = varianza(conte,promedio);
    auto varianza_respuesta = divi(suma_el,conte.size());
    return round(varianza_respuesta*100)/100;
};


int main() {

    vector<string> contenedor{"hola", "programacion","laptop"};
    variance(contenedor);
}