#include "Math.h"
#include <xmmintrin.h>

namespace Cookie::Util {

	void Math::MatrixMultiplication(float* matrixA, float* matrixB, float* result, uint32_t width, uint32_t height) {
		for (size_t i = 0; i < height; ++i) {
			for (size_t k = 0; k < width; ++k) {
				register float temp = matrixA[i * width + k];
				for (size_t j = 0; j < width; ++j) {
					result[i * width + j] += temp * matrixB[k * width + j];
				}
			}
		}
	}

	void Math::MatrixMultiplicationSIMD(float* matrixA, float* matrixB, float* result) {
		for (size_t i = 0; i < 4; ++i) {
			for (size_t j = 0; j < 4; ++j) {
				__m128 sum = _mm_setzero_ps();

				for (size_t k = 0; k < 4; ++k) {
					__m128 a = _mm_load_ps(matrixA + i * 4 + k);
					__m128 b = _mm_load_ps(matrixB + k * 4 + j);

					sum = _mm_add_ps(sum, _mm_mul_ps(a, b));
				}

				_mm_store_ps(result + i * 4 + j, sum);
			}
		}
	}

}