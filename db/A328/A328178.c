/*
 * A328178  a(n) is the minimal value of the expression d XOR (n/d) where d
 *          runs through the divisors of n and XOR denotes the bitwise XOR
 *          operator.
 * https://oeis.org/A328178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ d XOR (n/d) | d divides n }
 * Range:     u8 n <= 256, u16 n <= 9750, u32 n <= 9750, u64 n <= 9750,
 *            u128 n <= 9750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety, n>=1 in OEIS */

    seq_t best = 0;
    bool first = true;

    /* iterate d from 1 to sqrt(n) without overflow */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            seq_t val = (seq_t)d ^ (seq_t)q;
            if (first) {
                best = val;
                first = false;
            } else if (val < best) {
                best = val;
            }
        }
    }

    return best;
}
