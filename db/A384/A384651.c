/*
 * A384651  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of B(x)^k, where B(x)
 *          is the g.f. of A162661.
 * https://oeis.org/A384651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A384651: A square array of B(x)^k, read by antidiagonals.  */
/* Recurrence: A(0,k)=1, A(n,0)=0 (n>0), and for n>0,k>0:          */
/*   A(n,k) = Σ_{j=0}^{n} (k * C(2n-j+k, j) * A(n-j, j) / (2n-j+k))   */
/* Indexing: antidiagonals are read with (i,j) where i increases from 0 to d, j=d-i. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/binom.h"

static seq_t *table = NULL;   /* flat array A[n][k] */
static idx_t stride = 0;      /* current maximal n,k + 1 */

/* compute antidiagonal coordinates (i,j) for flat index n */
static void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    if (n == 0) {
        *i = 0; *j = 0; return;
    }
    /* approximate diagonal d */
    idx_t d = (idx_t)((sqrt((double)(8.0 * (double)n + 1.0)) - 1.0) / 2.0);
    while ((d + 1) * (d + 2) / 2 <= n) d++;
    while (d * (d + 1) / 2 > n) d--;
    idx_t r = n - d * (d + 1) / 2;
    *i = r;
    *j = d - r;
}

/* fill table up to indices [0..maxd][0..maxd] */
static void recompute_table(idx_t maxd)
{
    idx_t new_stride = maxd + 1;
    size_t new_size = (size_t)new_stride * (size_t)new_stride;
    seq_t *new_table = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_table) abort();

    for (idx_t n = 0; n <= maxd; ++n) {
        for (idx_t k = 0; k <= maxd; ++k) {
            seq_t val;
            if (n == 0) {
                /* A(0,k) = 1 for all k */
                val = 1;
            } else if (k == 0) {
                /* A(n,0) = 0 for n>0 */
                val = 0;
            } else {
                seq_t sum = 0;
                for (idx_t j = 0; j <= n; ++j) {
                    idx_t denom = 2 * n - j + k;
                    seq_t term = binom(denom, j);
                    term *= new_table[(n - j) * new_stride + j];
                    term *= (seq_t)k;
                    term /= (seq_t)denom;
                    sum += term;
                }
                val = sum;
            }
            new_table[n * new_stride + k] = val;
        }
    }

    free(table);
    table = new_table;
    stride = new_stride;
}

/* sequence value at index n */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    idx_t required = i > j ? i : j;
    if (stride == 0 || required + 1 > stride) {
        recompute_table(required);
    }
    return table[i * stride + j];
}
