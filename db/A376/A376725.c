/*
 * A376725  Expansion of 1/((1 - x^4 - x^5)^2 - 4*x^9).
 * https://oeis.org/A376725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Sum_{k=0..⌊n/4⌋} binomial(2k+2, 2n-8k+1)
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k * 4 <= n; ++k) {
        idx_t top    = 2 * k + 2;
        idx_t bottom = 2 * n - 8 * k + 1;
        seq_t c = binomial(top, bottom);
        sum += c;
    }
    /* The sum is always even, so a division by 2 is exact. */
    return sum >> 1;
}
