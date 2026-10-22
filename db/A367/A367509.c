/*
 * A367509  a(n) = Sum_{d|n} (2*d+1)^n.
 * https://oeis.org/A367509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (2*d+1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            seq_t base = (seq_t)(2 * d + 1);
            seq_t term = big_pow(base, (seq_t)n);
            sum += term;
        }
    }
    return sum;
}
