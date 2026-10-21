/*
 * A251049  Number of (n+1) X (2+1) 0..3 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with elements 0–3 such that for
 *            every 2×2 subblock the sum of diagonal elements a+d is NOT
 *            greater than the absolute difference of the antidiagonal
 *            elements |b−c|. Computed by transfer‑matrix exponentiation
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 46, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t m[64][64];
} matrix_t;

/* matrix multiplication C = A · B */
static void mat_mul(const matrix_t *A, const matrix_t *B, matrix_t *C)
{
    for (idx_t i = 0; i < 64; ++i) {
        for (idx_t j = 0; j < 64; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < 64; ++k)
                sum += A->m[i][k] * B->m[k][j];
            C->m[i][j] = sum;
        }
    }
}

/* the sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static matrix_t transition;

    /* build the transition matrix once */
    if (!initialized) {
        for (idx_t i = 0; i < 64; ++i)
            for (idx_t j = 0; j < 64; ++j)
                transition.m[i][j] = (seq_t)0;

        for (idx_t s = 0; s < 64; ++s) {
            int s0 = (s >> 4) & 3;
            int s1 = (s >> 2) & 3;
            int s2 =  s        & 3;
            for (idx_t t = 0; t < 64; ++t) {
                int t0 = (t >> 4) & 3;
                int t1 = (t >> 2) & 3;
                int t2 =  t        & 3;

                int diff0 = s1 > t0 ? s1 - t0 : t0 - s1;
                int diff1 = s2 > t1 ? s2 - t1 : t1 - s2;

                if ((s0 + t1 <= diff0) && (s1 + t2 <= diff1))
                    transition.m[s][t] = (seq_t)1;
            }
        }
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;  /* undefined below offset, but safe */

    /* exponentiate transition matrix to power n */
    matrix_t res, cur, tmp;
    const seq_t ONE = (seq_t)1;

    for (idx_t i = 0; i < 64; ++i)
        for (idx_t j = 0; j < 64; ++j)
            res.m[i][j] = (i == j) ? ONE : (seq_t)0;

    cur = transition;
    idx_t e = n;
    while (e) {
        if (e & 1) {
            mat_mul(&res, &cur, &tmp);
            res = tmp;
        }
        e >>= 1;
        if (e) {
            mat_mul(&cur, &cur, &tmp);
            cur = tmp;
        }
    }

    /* sum all entries of res to get a(n) */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 64; ++i)
        for (idx_t j = 0; j < 64; ++j)
            total += res.m[i][j];

    return total;
}
