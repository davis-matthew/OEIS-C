/*
 * A335156  Three-dimensional array A(m;r,s) = Sum_{i=0..r} Sum_{j=0..s}
 *          (-1)^(i+j+r+s)*binomial(r,i)*binomial(s,j)*binomial(i*j,m)
 *          displayed as a series of square slices read across rows (see
 *          Comments for details).
 * https://oeis.org/A335156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   A(m;r,s) = Sum_{i=0..r} Sum_{j=0..s} (-1)^(i+j+r+s) *
 *            binomial(r,i) * binomial(s,j) * binomial(i*j,m) The sequence is
 *            the concatenation of the entries of A(m;r,s) for m = 1,2,3, ...
 *            with 1 ≤ r ≤ m and 1 ≤ s ≤ m, read row‑by‑row (i.e. for a fixed
 *            m, r runs from 1 to m and inside that s runs from 1...
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* Find the block m such that the first term in block m is
       the first term with index T(m-1)+1, where
       T(m) = sum_{k=1}^m k^2 = m(m+1)(2m+1)/6. */
    idx_t m = 1;
    /* Use 128-bit intermediate to avoid overflow in the product */
    while ( (( (__int128)m * (m + 1) * (2 * m + 1)) / 6 ) < n )
        ++m;

    /* offset of n within block m (1‑based) */
    idx_t prev = ( ( (__int128)(m-1) * (m-1 + 1) * (2 * (m-1) + 1) ) / 6 );
    idx_t offset = n - prev;                 /* 1 ≤ offset ≤ m*m */

    /* Determine current indices r and s within the m×m square,
       reading rows (r fixed, s varying). */
    idx_t r = (offset - 1) / m + 1;          /* 1 ≤ r ≤ m */
    idx_t s = (offset - 1) % m + 1;          /* 1 ≤ s ≤ m */

    /* Compute A(m; r, s) as described, accumulating positive and
       negative contributions separately to avoid temporary
       negative intermediate values. */
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t i = 0; i <= r; ++i) {
        seq_t bin_r_i = binomial(r, i);      /* binomial(r,i) */
        for (idx_t j = 0; j <= s; ++j) {
            /* Skip terms where i*j < m because binomial(i*j,m) == 0 */
            if ((i * j) < m) continue;

            seq_t bin_s_j   = binomial(s, j);        /* binomial(s,j) */
            seq_t bin_ij_m  = binomial(i * j, m);    /* binomial(i*j,m) */

            /* Compute product of the three binomials. Use seq_mul
               to guarantee correctness for all build types. */
            seq_t prod = seq_mul(seq_mul(bin_r_i, bin_s_j), bin_ij_m);

            /* Sign factor (-1)^(i + j + r + s) */
            int sign = ((i + j + r + s) & 1) ? -1 : 1;

            if (sign > 0)
                sum_pos += prod;
            else
                sum_neg += prod;
        }
    }

    /* The final result is a non‑negative integer: sum_pos ≥ sum_neg. */
    return sum_pos - sum_neg;
}
