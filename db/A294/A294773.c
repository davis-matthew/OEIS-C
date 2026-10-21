/*
 * A294773  a(n) = Sum_{d|n} d^(d+n+1).
 * https://oeis.org/A294773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d+n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            sum += seq_pow((seq_t)d1, d1 + n + 1);
            if (d1 != n / i) {
                idx_t d2 = n / i;
                sum += seq_pow((seq_t)d2, d2 + n + 1);
            }
        }
    }
    return sum;
}
