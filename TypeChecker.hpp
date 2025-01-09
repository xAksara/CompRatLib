#pragma once
#include "Rational.hpp"
#include "Complex.hpp"

template<typename T>
struct is_rational : std::false_type {};

template<typename T>
struct is_rational<Rational<T>> : std::true_type {};

template<typename T>
struct is_complex : std::false_type {};

template<typename T1, typename T2>
struct is_complex<Complex<T1, T2>> : std::true_type {};