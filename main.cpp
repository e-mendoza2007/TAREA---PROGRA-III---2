#include<iostream>
#include<vector>
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
concept Divisible = requires ( T a , std :: size_t n ) {
    { a / n } -> same_as <T >;
};

template < Iterable C >
requires Addable < typename C :: value_type >
auto sum ( const C & container )
{
    using T = typename C :: value_type ;
    T result {}; // Para tipos numericos (int , double , float )
    // se inicializa en 0.
    // Para clases se llama al constructor por defecto
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
auto mean(C contenedor) {

     auto suma = sum(contenedor);
    auto promedio =  divi(suma, contenedor.size());
return promedio;
}

int main() {

    vector <double> a{1.0,2.0,3.0};
    auto m = mean(a);
    cout << m << endl ;
}