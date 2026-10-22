/*
 * A338528  Number of partitions of k*n into two parts (s,t) such that s <= t
 *          and t | k*s for k = 1..n.
 * https://oeis.org/A338528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = Sum_{k=1..n} [# of divisors u of k^2*n
   such that k < u <= 2k] */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* k^2 * n fits in 64 bits for the given value range (n <= 10000) */
        uint64_t N = (uint64_t)k * (uint64_t)k * (uint64_t)n;
        for (idx_t u = k + 1; u <= 2 * k; ++u) {
            if (N % (uint64_t)u == 0)
                ++total;
        }
    }
    return total;
}
