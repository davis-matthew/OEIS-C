/*
 * A248417  Sum of n-th powers of the roots of x^3 +25* x^2 + 31*x - 1.
 * https://oeis.org/A248417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -25*a(n-1) - 31*a(n-2) + a(n-3); Initial terms: a(0)=3,
 *            a(1)=-25, a(2)=563
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_a0 = 3;
static const seq_t init_a1 = -25;
static const seq_t init_a2 = 563;

/* 3x3 matrix of seq_t values */
typedef struct {
    seq_t m[3][3];
} mat3_t;

/* Identity matrix */
static mat3_t mat3_identity(void) {
    mat3_t I = {0};
    I.m[0][0] = 1; I.m[0][1] = 0; I.m[0][2] = 0;
    I.m[1][0] = 0; I.m[1][1] = 1; I.m[1][2] = 0;
    I.m[2][0] = 0; I.m[2][1] = 0; I.m[2][2] = 1;
    return I;
}

/* Matrix multiplication: res = A * B */
static mat3_t mat3_mult(const mat3_t *A, const mat3_t *B) {
    mat3_t R = {0};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 3; ++k) {
                sum += A->m[i][k] * B->m[k][j];
            }
            R.m[i][j] = sum;
        }
    }
    return R;
}

/* Power of a matrix: M^e */
static mat3_t mat3_pow(mat3_t base, idx_t e) {
    mat3_t result = mat3_identity();
    while (e > 0) {
        if (e & 1) {
            result = mat3_mult(&result, &base);
        }
        base = mat3_mult(&base, &base);
        e >>= 1;
    }
    return result;
}

/* Multiply matrix by vector (size 3) */
static seq_t mat3_mul_vec_first(const mat3_t *M, const seq_t vec[3]) {
    /* Return the first component of M * vec */
    seq_t value = 0;
    for (int j = 0; j < 3; ++j) {
        value += M->m[0][j] * vec[j];
    }
    return value;
}

/* External function requested by the problem */
seq_t a(idx_t n) {
    if (n == 0) return init_a0;
    if (n == 1) return init_a1;
    if (n == 2) return init_a2;

    /* Companion matrix for the recurrence:
       [a(n)]   = [-25 -31  1] * [a(n-1)]
       [a(n-1)]   [ 1  0  0]   [a(n-2)]
       [a(n-2)]   [ 0  1  0]   [a(n-3)]   */
    mat3_t M;
    M.m[0][0] = -25; M.m[0][1] = -31; M.m[0][2] = 1;
    M.m[1][0] = 1;   M.m[1][1] = 0;   M.m[1][2] = 0;
    M.m[2][0] = 0;   M.m[2][1] = 1;   M.m[2][2] = 0;

    /* Compute M^(n-2) */
    mat3_t Mp = mat3_pow(M, n - 2);

    /* Base vector V(2) = [a(2), a(1), a(0)]^T */
    seq_t base[3] = {init_a2, init_a1, init_a0};

    /* Resulting a(n) is the first component of Mp * base */
    return mat3_mul_vec_first(&Mp, base);
}
