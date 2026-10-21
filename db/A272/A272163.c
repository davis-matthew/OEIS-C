/*
 * A272163  a(n) = Product_{k=0..n} (n^2-k)^k.
 * https://oeis.org/A272163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} (n^2 - k)^k
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t res  = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = n_sq - (seq_t)k;
        seq_t term = big_pow(base, (seq_t)k);
        res = res * term;           /* multiplication in seq_t type */
    }
    return res;
}
