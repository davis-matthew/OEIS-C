/*
 * A260934  Sum of evil divisors of n. For evil numbers see A001969.
 * https://oeis.org/A260934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of evil divisors of n. An evil number is a number
 *            with an even number of 1‑bits in its binary representation
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t u = (uint64_t)n;
    seq_t sum = 0;
    for (uint64_t d = 1; d * d <= u; ++d) {
        if (u % d == 0) {
            uint64_t e = u / d;
            if ((__builtin_popcountll(d) & 1) == 0) sum += (seq_t)d;
            if (e != d && ((__builtin_popcountll(e) & 1) == 0))
                sum += (seq_t)e;
        }
    }
    return sum;
}
