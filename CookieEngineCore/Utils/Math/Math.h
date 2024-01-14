#pragma once
#include "Common/Common.h"

#define PI 3.14159265358979323846

namespace Cookie::Util {
	
	class Math {
	public:
	
		/**
		 * Generic matrix multiplication, support all kinds of size 
		 */
		static void MatrixMultiplication(float* matrixA, float* matrixB, float* result, uint32_t width, uint32_t height);

		/**
		 * To support 4 * 4 matrix multiplication, other size may lead to error
		 */
		static void MatrixMultiplicationSIMD(float* matrixA, float* matrixB, float* result);

	};

}