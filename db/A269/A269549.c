/*
 * A269549  Expansion of (-x^2 + 298*x - 1)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-3) - 99*a(n-2) + 99*a(n-1) with initial terms a(0)=1,
 *            a(1)=-199, a(2)=-19799
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 4, i64 n <= 9, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recurrence matrix for the linear recurrence a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) */
static const seq_t M[3][3] = {
    {(seq_t)99, (seq_t)-99, (seq_t)1},
    {(seq_t)1,  (seq_t)0,  (seq_t)0},
    {(seq_t)0,  (seq_t)1,  (seq_t)0}
};

/* Initial vector [a2, a1, a0]^T */
static const seq_t init[3] = {(seq_t)-19799, (seq_t)-199, (seq_t)1};

/* Matrix multiplication: C = A * B */
static void mat_mult(seq_t C[3][3], const seq_t A[3][3], const seq_t B[3][3]) {
    seq_t tmp[3][3] = {{(seq_t)0}};
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < 3; ++k) {
                sum += A[i][k] * B[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            C[i][j] = tmp[i][j];
}

/* Fast exponentiation: R = M^e */
static void mat_pow(seq_t R[3][3], const seq_t base[3][3], idx_t e) {
    seq_t res[3][3] = {
        {(seq_t)1, (seq_t)0, (seq_t)0},
        {(seq_t)0, (seq_t)1, (seq_t)0},
        {(seq_t)0, (seq_t)0, (seq_t)1}
    };
    seq_t cur[3][3];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            cur[i][j] = base[i][j];

    while (e > 0) {
        if (e & (idx_t)1) {
            seq_t tmp[3][3];
            mat_mult(tmp, res, cur);
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    res[i][j] = tmp[i][j];
        }
        e >>= 1;
        if (e) {
            seq_t tmp[3][3];
            mat_mult(tmp, cur, cur);
            for (int i = 0; i < 3; ++i)
                for (int j = 0; j < 3; ++j)
                    cur[i][j] = tmp[i][j];
        }
    }
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            R[i][j] = res[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-199;
    if (n == 2) return (seq_t)-19799;

    /* Compute M^(n-2) */
    seq_t Mpow[3][3];
    mat_pow(Mpow, M, n - 2);

    /* a(n) = first row of Mpow * init vector */
    seq_t result = (seq_t)0;
    for (int k = 0; k < 3; ++k) {
        result += Mpow[0][k] * init[k];
    }
    return result;
}
