#ifndef COMPLEX_H
#define COMPLEX_H

// Class representing a complex number
struct Complex {
	double re; // Real part
	double im; // Imaginary part

	// Default constructor doing no initialization
	Complex() {}
	// Constructor from real value or from real and imaginary values
	Complex(double _re,double _im=0) : re(_re), im(_im) {}
	// Copy constructor
	Complex(const Complex& that) : re(that.re), im(that.im) {}
	// Destructor
	~Complex() {}

	// Two complex numbers are equal iff their real parts are equal and their imaginary parts are equal
	bool operator==(const Complex& that) const {
		return re==that.re && im==that.im;
	}
	bool operator!=(const Complex& that) const {
		return re!=that.re || im!=that.im;
	}

	// +(a+bi) = a+bi
	Complex operator+() const {
		return Complex(re,im);
	}
	// (a+bi) + (c+di) = (a+c) + (b+d)i
	Complex operator+(const Complex& that) const {
		return Complex(re+that.re,im+that.im);
	}
	Complex& operator+=(const Complex& that) {
		re += that.re;
		im += that.im;
		return *this;
	}

	// -(a+bi) = -a-bi
	Complex operator-() const {
		return Complex(-re,-im);
	}
	// (a+bi) - (c+di) = (a-c) + (b-d)i
	Complex operator-(const Complex& that) const {
		return Complex(re-that.re,im-that.im);
	}
	Complex& operator-=(const Complex& that) {
		re -= that.re;
		im -= that.im;
		return *this;
	}

	// Return true iff not equal to zero
	bool operator!() const {
		return re!=0 || im!=0;
	}

	// Access as component 0 or 1, e.g. c[0] for real part or c[1] for imaginary part
	double operator[](size_t i) const {
		return i ? im : re;
	}
	double& operator[](size_t i) {
		return i ? im : re;
	}

	// (a+bi)*(c+di) = ac + adi + bci + bdi^2 = (ac - bd) + i(ad + bc)
	Complex operator*(const Complex& that) const {
		return Complex(re*that.re - im*that.im,re*that.im + im*that.re);
	}
	Complex& operator*=(const Complex& that) {
		double newRe = re*that.re - im*that.im;
		double newIm = re*that.im + im*that.re;
		re = newRe;
		im = newIm;
		return *this;
	}

	// magnitude^2 = re^2 + im^2, by Pythagorean theorem
	double magnitude() const {
		return sqrt(re*re + im*im);
	}
	double magnitude2() const {
		return re*re + im*im;
	}

	// tan(phase) = im/re
	// phase = atan2(im,re)
	double phase() const {
		return atan2(im,re);
	}

	// cos(phase) = re/magnitude; sin(phase) = im/magnitude
	// re = magnitude*cos(phase); im = magnitude*sin(phase)
	static Complex fromMagnitudeAndPhase(double magnitude,double phase) {
		return Complex(magnitude*cos(phase),magnitude*sin(phase));
	}
};

#endif
