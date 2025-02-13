#include <immintrin.h>
#include <math.h>

float formula1(float *x, unsigned int length) {

    // TODO: Think about considering removing lines 7-9.
    __m128 x_vec = _mm_setzero_ps(); // x_vec <= [0.0, 0.0, 0.0, 0.0].
    __m128 sqrt_x_sum = _mm_setzero_ps(); // sqrt_x_sum <= [0.0, 0.0, 0.0, 0.0].
    __m128 sigma = _mm_setzero_ps(); // sigma <= [0.0, 0.0, 0.0, 0.0].
    __m128 sqrt_x = _mm_setzero_ps();
    __m128 x_2 = _mm_setzero_ps();
    __m128 pai = _mm_set1_ps(1.0f); // pai <= [1.0, 1.0, 1.0, 1.0].
    __m128 ones = _mm_set1_ps(1.0f); // ones <= [1.0, 1.0, 1.0, 1.0].

    for (long i = 0; i < length; i += 4) {
        /* Load 4 floats at a time */
        x_vec = _mm_load_ps(x + i);

        // Numerator section:
        sqrt_x = _mm_sqrt_ps(x_vec);
        sqrt_x_sum = _mm_add_ps(sqrt_x_sum, sqrt_x);

        // Denominator section:
        x_2 = _mm_mul_ps(x_vec, x_vec); // x_2 <= [x_vec[0]^2, x_vec[1]^2, x_vec[2]^2, x_vec[3]^2].
        x_2 = _mm_add_ps(x_2, ones); // x_2 <= [x_vec[0]^2 + 1, x_vec[1]^2 + 1, x_vec[2]^2 + 1, x_vec[3]^2 + 1].
        pai = _mm_mul_ps(pai, x_2);
    }

    // Sigma <= sqrt(x1) + sqrt(x2) + ... + sqrt(xn).
    sigma = _mm_hadd_ps(sqrt_x_sum, sqrt_x_sum);
    sigma = _mm_hadd_ps(sigma, sigma);


    float sigma_scalar = _mm_cvtss_f32(sigma);
    // Calculate the cube root of the sum of square roots.
     float cbrt_sigma = cbrtf(sigma_scalar);

    float productResult[4];
    _mm_storeu_ps(productResult, pai);

    float denominator = productResult[0] * productResult[1] * productResult[2] * productResult[3];

    float final_result = cbrt_sigma / denominator;
    final_result = final_result + 1;

     return sqrtf(final_result);
}
