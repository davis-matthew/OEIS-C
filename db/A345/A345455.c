/*
 * A345455  a(n) = Sum_{k=0..n} binomial(5*n+1,5*k).
 * https://oeis.org/A345455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21*a(n-1) + 353*a(n-2) - 32*a(n-3); a(0) = 1, a(1) = 7,
 *            a(2) = 474 The function uses matrix exponentiation to obtain
 *            O(log n) time
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* size_t */

typedef struct { seq_t m[3][3]; } mat3;

/* Matrix multiplication */
static mat3 mat3_mul(mat3 a, mat3 b) {
    mat3 r;
    for (idx_t i = 0; i < 3; ++i) {
        for (idx_t j = 0; j < 3; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < 3; ++k) {
                sum = sum + a.m[i][k] * b.m[k][j];
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

/* Identity matrix */
static mat3 mat3_identity(void) {
    mat3 I;
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            I.m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
    return I;
}

/* Fast exponentiation of a 3×3 matrix (exp >= 0) */
static mat3 mat3_pow(mat3 base, idx_t exp) {
    mat3 result = mat3_identity();
    while (exp > 0) {
        if (exp & 1ULL)
            result = mat3_mul(result, base);
        base = mat3_mul(base, base);
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)474;

    /* Transition matrix for the recurrence:
         [21  353 -32]
         [ 1    0    0]
         [ 0    1    0]
    */
    mat3 M;
    M.m[0][0] = (seq_t)21;  M.m[0][1] = (seq_t)353; M.m[0][2] = (seq_t)(-32);
    M.m[1][0] = (seq_t)1;   M.m[1][1] = (seq_t)0;   M.m[1][2] = (seq_t)0;
    M.m[2][0] = (seq_t)0;   M.m[2][1] = (seq_t)1;   M.m[2][2] = (seq_t)0;

    /* Compute M^(n-2) */
    mat3 P = mat3_pow(M, n - 2);

    /* Base vector [a(2), a(1), a(0)]^T */
    seq_t base_vec[3];
    base_vec[0] = (seq_t)474;
    base_vec[1] = (seq_t)7;
    base_vec[2] = (seq_t)1;

    /* First element of P * base_vec gives a(n) */
    seq_t result = (seq_t)0;
    for (idx_t j = 0; j < 3; ++j) {
        result = result + P.m[0][j] * base_vec[j];
    }
    return result;
}
