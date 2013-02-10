#ifndef MAIN_H
#define MAIN_H

#define DEBUG 1
#if DEBUG
#define IF_DEBUG(x) x
#else
#define IF_DEBUG(x)
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#define M_TAU      6.28318530717958647693
#define M_1_SQRTPI 0.564189583547756286948
#define M_SQRT3    1.73205080756887729353
#define M_SQRT6    2.44948974278317809820

#include <stdint.h>

typedef int8_t		int8;
typedef uint8_t		uint8;
typedef int16_t		int16;
typedef uint16_t	uint16;
typedef int32_t		int32;
typedef uint32_t	uint32;
typedef int64_t		int64;
typedef uint64_t	uint64;

typedef uint32_t	uint;

#include <xutility>
#include <vector>
#include <thread>

inline int getNumProcessors() {
	return std::thread::hardware_concurrency();
}

inline int correctFDiv(int numerator,int denominator) {
	int q = numerator / denominator;
	int r = numerator % denominator;
	return q - (r < 0);
}

inline int correctRDiv(int numerator,int denominator) {
	return correctFDiv(numerator + (denominator>>1), denominator);
}

inline int64 correctFloor(double a) {
	int64 b = (int64)a;
	return b - (b > a);
}

inline int64 correctCeiling(double a) {
	int64 b = (int64)a;
	return b + (b < a);
}

inline int64 correctRound(double a) {
	return (int64)(a + ((a < 0) ? -0.5 : 0.5));
}

inline bool isTolEqual(double a,double b,double tol=1e-6) {
	return (a-b <= tol) && (a-b >= -tol);
}

using std::max;
using std::min;

#include <SDL.h>
// SDL_main.h (included from SDL.h) redefines main as SDL_main, which we don't want
#undef main

#include "Complex.h"

#endif
