/*
 * A384758  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B(x)^k, where
 *          B(x) is the e.g.f. of A384757.
 * https://oeis.org/A384758
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A(n,k) = (-1)^n * k * Σ_{j=1..n} (n-j+k)^(j-1) * binomial(n,j) *
 *            A(n-j,j); with A(n,0)=0^n (0^0=1) and A(0,k)=1. The sequence
 *            a(n) is the antidiagonal reading of this square array
 * Range:     i8 n <= 25, i16 n <= 42, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

#ifdef __cplusplus
#include <vector>
#endif

/* Return the n-th term of sequence A384758. */
seq_t a(idx_t n)
{
    /* Find the diagonal index s such that (s+1)(s+2)/2 > n. */
    idx_t s = 0;
    while ( ((s + 1ULL) * (s + 2ULL)) / 2ULL <= n ) ++s;

    /* Offset along this diagonal. */
    idx_t offset = n - (s * (s + 1ULL)) / 2ULL;
    idx_t k = s - offset;          /* column index */
    idx_t i = offset;              /* row   index */

#ifdef __cplusplus
    /* C++ case: use std::vector for temporary storage. */
    std::vector<std::vector<seq_t>> A(s + 1, std::vector<seq_t>(s + 1));

    for (idx_t t = 0; t <= s; ++t) {
        for (idx_t row = 0; row <= t; ++row) {
            idx_t col = t - row;
            if (row == 0) {
                A[0][col] = (seq_t)1;            /* A(0,k)=1 */
            } else if (col == 0) {
                A[row][0] = (row == 0) ? (seq_t)1 : (seq_t)0;  /* 0^row */
            } else {
                seq_t sum = 0;
                for (idx_t j = 1; j <= row; ++j) {
                    seq_t base = (seq_t)(row - j + col);
                    seq_t powv  = big_pow(base, (seq_t)(j - 1));
                    seq_t bin   = binomial(row, j);
                    seq_t term  = powv * bin;
                    term        = term * A[row - j][j];
                    sum += term;
                }
                seq_t res = sum;
                if (row & 1U) res = -res;          /* multiply by (-1)^row */
                res *= (seq_t)col;
                A[row][col] = res;
            }
        }
    }
    return A[i][k];

#else
    /* C case: use manual dynamic memory. */
    seq_t **A = (seq_t**)malloc((s + 1) * sizeof(seq_t*));
    for (idx_t r = 0; r <= s; ++r) {
        A[r] = (seq_t*)calloc((s + 1), sizeof(seq_t));
    }

    for (idx_t t = 0; t <= s; ++t) {
        for (idx_t row = 0; row <= t; ++row) {
            idx_t col = t - row;
            if (row == 0) {
                A[0][col] = (seq_t)1;                /* A(0,k)=1 */
            } else if (col == 0) {
                A[row][0] = (row == 0) ? (seq_t)1 : (seq_t)0;  /* 0^row */
            } else {
                seq_t sum = 0;
                for (idx_t j = 1; j <= row; ++j) {
                    seq_t base = (seq_t)(row - j + col);
                    seq_t powv  = big_pow(base, (seq_t)(j - 1));
                    seq_t bin   = binomial(row, j);
                    seq_t term  = powv * bin;
                    term        = term * A[row - j][j];
                    sum += term;
                }
                seq_t res = sum;
                if (row & 1U) res = -res;              /* (-1)^row */
                res *= (seq_t)col;
                A[row][col] = res;
            }
        }
    }

    seq_t result = A[i][k];

    for (idx_t r = 0; r <= s; ++r) {
        free(A[r]);
    }
    free(A);
    return result;
#endif
}
