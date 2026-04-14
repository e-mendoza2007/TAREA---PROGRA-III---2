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

int main() {


    cout << "################## Pregunta 2 ################\n";
    vector <double> a{1.0,2.0,3.0};
    cout << "Vector: "<< endl;
    imprimir(a);
    auto m = mean(a);
    cout << "Promedio: " << m << endl ;
    cout << "################## Pregunta 3 ################\n";
    vector<double> data{1.0,2.0,3.0};
    cout << "Vector: "<< endl;
    imprimir(data);
    auto v = variance(data);
    cout << "Varianza: " << v << endl ;
    cout << "################## Pregunta 4 ################\n";
    vector <double> b{1,2.7,0.3};
    cout << "Vector: "<< endl;
    imprimir(b);
    auto maximo_ = maximo(b);
    cout << "Maximo: " << maximo_ << endl ;
}