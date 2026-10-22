/*
 * A392254  a(n) = Sum_{k=0..floor(n/2)} (k+1) * binomial(k,3*(n-2*k)).
 * https://oeis.org/A392254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (k+1) * binom(k, 3*(n-2*k))
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 64, u64 n <= 131,
 *            u128 n <= 267, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = 3 * (n - 2 * k);
        /* binom returns 0 if m > k or m < 0 */
        seq_t term = ((seq_t)(k + 1)) * binom(k, m);
        result += term;
    }
    return result;
}
