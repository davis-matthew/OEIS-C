/*
 * A302346  Triangle read by rows. Number of invertible linear operators T on
 *          an n-dimensional vector space over GF(2) such that T(U) = U for
 *          some given k-dimensional subspace U.
 * https://oeis.org/A302346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row index with r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2, k = n - r(r+1)/2. T(r,k) = GL(k,2) * GL(r-k,2) *
 *            2^(k*(r-k)), with GL(m,2) = ∏_{i=0}^{m-1} (2^m – 2^i)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

/* Compute |GL(m,2)| = ∏_{i=0}^{m-1} (2^m – 2^i). */
static seq_t gl_value(idx_t m)
{
    seq_t prod = 1;
    seq_t two_m = pow_uint(2, m);
    for (idx_t i = 0; i < m; ++i) {
        seq_t term = two_m - pow_uint(2, i);
        prod = prod * term;
    }
    return prod;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (1) {
        seq_t next_tri = ((seq_t)(r + 1) * (seq_t)(r + 2)) / 2;
        if (next_tri <= (seq_t)n)
            ++r;
        else
            break;
    }

    /* Start index of row r. */
    idx_t start = (idx_t)(((seq_t)r * (seq_t)(r + 1)) / 2);
    idx_t k = n - start;              /* column within the row */

    seq_t val1 = gl_value(k);
    seq_t val2 = gl_value(r - k);
    idx_t  exp  = k * (r - k);
    seq_t pow_term = pow_uint(2, exp);

    return val1 * val2 * pow_term;
}
