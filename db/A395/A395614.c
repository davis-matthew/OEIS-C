/*
 * A395614  a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(2*n,n+4*k).
 * https://oeis.org/A395614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n,k) * binomial(2*n,n+4*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        /* compute binomial(n, k) */
        seq_t bin_n_k = binomial(n, k);
        /* compute binomial(2*n, n + 4*k) */
        seq_t bin_2n_n4k = binomial(2 * n, n + 4 * k);
        /* add product to running total */
        sum += bin_n_k * bin_2n_n4k;
    }
    return sum;
}
