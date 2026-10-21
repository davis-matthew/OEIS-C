/*
 * A262843  Inverse Moebius transform of n^(n-1).
 * https://oeis.org/A262843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum{d|n} d^(d-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            seq_t term1 = big_pow((seq_t)d, (seq_t)(d - 1));
            sum += term1;
            idx_t other = n / d;
            if (other != d) {
                seq_t term2 = big_pow((seq_t)other, (seq_t)(other - 1));
                sum += term2;
            }
        }
    }
    return sum;
}
