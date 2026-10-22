/*
 * A377247  a(n) is the largest k such that the sum of the first k divisors of
 *          n is at most n.
 * https://oeis.org/A377247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest k such that the sum of the first k divisors
 *            of n is ≤ n (search)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* n ≥ 1 by definition */

    uint64_t sum = 0;
    seq_t k = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            uint64_t dv = (uint64_t)d;
            if (sum + dv > (uint64_t)n) {
                return k;
            }
            sum += dv;
            ++k;
        }
    }
    return k;
}
