/*
 * A320354  Square array A(n,k), n >= 0, k >= 1, read by antidiagonals: A(n,k)
 *          = Product_{j=1..n} (k^j - 1).
 * https://oeis.org/A320354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i,k) where k = smallest integer with k*(k+1)/2 > n, i =
 *            n - T_{k-1} (T_{k-1} = (k-1)*k/2), j = k - i, A(i,j) =
 *            ∏_{s=1..i} (j^s - 1), with A(0,*) = 1. Sequence a(n) is read by
 *            antidiagonals of the square array A(n,k)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 41, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   triangular_upper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include "../utils/triangular_upper.h"

static inline idx_t triangular(idx_t k) {
    /* Return k*(k+1)/2 using 128-bit intermediate to avoid overflow. */
    return ( ( (__int128)k * (k + 1) ) >> 1 );
}

seq_t a(idx_t n) {
    /* Determine the antidiagonal (k) for index n. */
    idx_t k = triangular_upper(n);
    idx_t t_prev = triangular(k - 1);   /* T_{k-1} */
    idx_t i = n - t_prev;              /* row index */
    idx_t j = k - i;                   /* column index (>=1) */

    /* Compute the product ∏_{s=1..i} (j^s - 1). */
    seq_t product = 1;
    if (i > 0) {
        seq_t pow_val = (seq_t)j;      /* j^1 */
        for (idx_t s = 1; s <= i; ++s) {
            /* term = j^s - 1 */
            seq_t term = pow_val - 1;
            product *= term;
            /* update pow_val = j^(s+1) */
            pow_val *= (seq_t)j;
        }
    }
    return product;
}
