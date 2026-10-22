/*
 * A384742  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B(x)^k, where
 *          B(x) is the e.g.f. of A384740.
 * https://oeis.org/A384742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A384742
    Formula for the square array A(n,k):
        A(n,0) = (n==0) ? 1 : 0
        A(n,k) = k * Σ_{j=1..n} (3*n - 3*j + k)^(j-1) * binomial(n,j) * A(n-j,j)

    The sequence a(n) is read by antidiagonals of this array:
        let  s = floor((√(8*n+1)-1)/2)
        let  t = n - s*(s+1)/2      // index of the row in that diagonal
        let  k = s - t              // column index in that diagonal
        a(n) = A(t,k)
*/

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"

static seq_t **A = NULL;          /* 2‑D table of A(n,k) */
static idx_t  max_dim = 0;        /* current table dimension (size of one side) */

/* Find the diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
static idx_t diagonal_of(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Exponential search for an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    /* Binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Recompute the whole table up to dimension "dim" */
static void recompute_table(idx_t dim)
{
    /* Free any existing table */
    if (max_dim) {
        for (idx_t i = 0; i < max_dim; ++i) free(A[i]);
        free(A);
    }
    /* Allocate new table */
    A = (seq_t**)malloc(dim * sizeof(seq_t*));
    for (idx_t i = 0; i < dim; ++i) A[i] = (seq_t*)calloc(dim, sizeof(seq_t));

    /* Fill the table */
    for (idx_t n = 0; n < dim; ++n) {
        /* Row 0 is all 1's */
        if (n == 0) {
            for (idx_t k = 0; k < dim; ++k) A[n][k] = 1;
            continue;
        }
        /* A(n,0) = 0 for n>0 */
        A[n][0] = 0;
        for (idx_t k = 1; k < dim; ++k) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= n; ++j) {
                seq_t base = (seq_t)(3*n - 3*j + k);
                /* compute base^(j-1) */
                seq_t pow = 1;
                for (idx_t m = 0; m < j-1; ++m) pow *= base;
                seq_t term = pow;
                term *= binomial(n, j);
                term *= A[n-j][j];
                sum += term;
            }
            A[n][k] = k * sum;
        }
    }
    max_dim = dim;
}

seq_t a(idx_t n)
{
    /* Determine the diagonal and offset */
    idx_t s = diagonal_of(n);
    idx_t t = n - s*(s+1)/2;      /* row index    */
    idx_t k = s - t;              /* column index */

    /* Ensure the table is large enough */
    if (s + 1 > max_dim) recompute_table(s + 1);

    return A[t][k];
}
