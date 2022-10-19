#include "complex.hpp"
#include <cmath>

  // Constructors
  Complex::Complex() : a{0}, b{0}{}
  Complex::Complex(float a, float b): a{a}, b{b}{}

  //operators
  Complex Complex::operator+(const Complex &other){
		return Complex(a + other.a, b + other.b);
	}

  Complex Complex::operator-(const Complex &other){
    return Complex(a - other.a, b - other.b);
  }

  Complex Complex::operator*(float x){
    return Complex(a * x, b * x);
  }

  Complex operator*(float x, const Complex& other){
    return Complex(other.a * x, other.b * x);
  }

  //absolute value of a complex number
	float Complex::abs_val(){
    return static_cast<float>(std::sqrt(a * a + b * b));
  }

	//sorting complex nums with their absolute value
	void sorting(Complex array[] , int n){
      Complex key;
      float sorting_value;
      int j;
   for(int i = 1; i<n; ++i) {
      key = array[i]; //take complex number
      sorting_value = array[i].abs_val();//take value
      j = i;
      while(j > 0 && array[j-1].abs_val()>sorting_value) {
         array[j] = array[j-1];
         j--;
      }
      array[j] = key;   //insert in right place
   }
	}


