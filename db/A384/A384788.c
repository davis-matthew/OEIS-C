/*
 * A384788  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B(x)^k, where
 *          B(x) is the e.g.f. of A384787.
 * https://oeis.org/A384788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(n,0) = 0^n (so A(0,0)=1, A(n>0,0)=0); A(n,k) = k * Σ_{j=1..n}
 *            2^(n-j) * (n+k)^(j-1) * binomial(n,j) * A(n-j,j) The flat
 *            sequence a(n) is obtained by reading the square array A(n,k) by
 *            antidiagonals downwards (first A(0,0), then A(0,1),A(1,0), then
 *            A(0,2),A(1,1),A(2,0), ...)
 * Range:     u8 n <= 17, u16 n <= 25, u32 n <= 42, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial, index_to_coord
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/index_to_coord.h"

/* Compute the sequence value a(n). */
seq_t a(idx_t n)
{
    idx_t i, j;
    index_to_coord(n, &i, &j);
    idx_t s = i + j;                 /* maximum diagonal sum needed */

    /* Allocate a (s+1) x (s+1) 2‑D array. */
    seq_t **A = (seq_t **)malloc((s + 1) * sizeof(seq_t *));
    for (idx_t r = 0; r <= s; ++r)
        A[r] = (seq_t *)calloc((s + 1), sizeof(seq_t));

    /* Base row: A(0,k) = 1 for all k. */
    for (idx_t k = 0; k <= s; ++k)
        A[0][k] = (seq_t)1;

    /* Compute rows n = 1 .. s. */
    for (idx_t row = 1; row <= s; ++row) {
        A[row][0] = (seq_t)0;                 /* A(n,0) = 0 for n>0 */
        for (idx_t col = 1; col <= s; ++col) {
            seq_t sum = (seq_t)0;
            for (idx_t jsel = 1; jsel <= row; ++jsel) {
                seq_t pow2  = big_pow((seq_t)2, (seq_t)(row - jsel));
                seq_t pow_nk = big_pow((seq_t)(row + col), (seq_t)(jsel - 1));
                seq_t bin     = binomial(row, jsel);
                seq_t sub = pow2;
                sub *= pow_nk;
                sub *= bin;
                sub *= A[row - jsel][jsel];
                sum += sub;
            }
            A[row][col] = sum * (seq_t)col;
        }
    }

    seq_t result = A[i][j];

    /* Free the allocated array. */
    for (idx_t r = 0; r <= s; ++r)
        free(A[r]);
    free(A);

    return result;
}
