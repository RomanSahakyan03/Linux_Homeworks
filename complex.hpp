#ifndef COMPLEX
#define COMPLEX

class Complex{
    private:

	      float a; //real part of complex number
        float b; //imaginary part of complex number

    public:

	// Default Constructor
  Complex();

  // Constructor
	Complex(float a, float b);

	//operators
	Complex operator+(const Complex &other);

	Complex operator-(const Complex &other);

	Complex operator*(float x);

  friend Complex operator*(float x, const Complex& other);

	//absolute value of a complex number
	float abs_val();

};

	//operator which cannot be a member of class
  Complex operator*(float x, const Complex& other);

	//sorting algorithm
	void sorting(Complex array[], int n);

#endif

