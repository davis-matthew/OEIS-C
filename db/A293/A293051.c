/*
 * A293051  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(Sum_{i=0..k} x^i/i! -
 *          exp(x)).
 * https://oeis.org/A293051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = A(i,j) where A(i,j) is defined by the recurrence * A(0,k)
 *            = 1 * A(n,k) = 0 for 0 < n <= k * A(n,k) = - ∑_{i=k}^{n-1}
 *            binom(n-1,i) * A(n-1-i,k) for n > k * The linear index n is
 *            converted to coordinates (i,j) by reading the * square array by
 *            antidiagonals: i is the row (offset), j the colum...
 * Range:     i8 n <= 52, i16 n <= 102, i32 n <= 207, i64 n <= 493,
 *            i128 n <= 1272, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"

#define MAX_N 300  /* sufficient for n up to about 10000 */

static seq_t table[MAX_N][MAX_N];
static bool known[MAX_N][MAX_N] = {0};

/* Recursive computation of A(n,k) with memoisation. */
static seq_t A_rec(idx_t n, idx_t k)
{
    if (known[n][k]) return table[n][k];

    seq_t result;
    if (n == 0) {
        result = (seq_t)1;                       /* A(0,k) = 1 */
    } else if (n <= k) {
        result = (seq_t)0;                       /* A(n,k) = 0 for 0 < n <= k */
    } else {
        seq_t sum = 0;
        for (idx_t i = k; i <= n-1; ++i) {
            seq_t b   = binom(n-1, i);          /* binom(n-1,i) */
            seq_t val = A_rec(n-1-i, k);        /* A(n-1-i,k)  */
            sum += b * val;
        }
        result = -sum;                            /* A(n,k) = -sum */
    }

    known[n][k] = true;
    table[n][k] = result;
    return result;
}

/* Convert a linear index n into (row,column) coordinates (i,j). */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    idx_t start  = d * (d + 1) / 2; /* first index of diagonal d */
    idx_t offset = n - start;       /* position within the diagonal */
    *i = offset;                    /* row index  */
    *j = d - offset;                /* column index */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return A_rec(i, j);
}
