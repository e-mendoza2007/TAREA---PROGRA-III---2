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

template < Addable T >
T sum( C) {

    T sum = begin(C);
    for (const auto& x : C) {
        sum += x;
    }
    return sum;
}

template < Divisible D>
D divi( D s , size_t n) {

    D d = s / n;
    return d;
}

template < Iterable C >
auto mean(C contenedor) {

    auto suma = sum(contenedor);
    auto promedio =  divi(suma, contenedor.size());
return promedio;
}

int main() {

    vector <double> a{1.0,2.0,3.0};
    auto m = mean(a);
}