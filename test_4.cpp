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

int main() {


        vector < std :: string > data {"a", "b", "c"};
        auto v =  max ( data ) ;


}