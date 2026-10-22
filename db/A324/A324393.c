/*
 * A324393  a(n) is the number of such divisors d of n that A000120(d) does
 *          not divide n, where A000120(d) gives the binary weight of d.
 * https://oeis.org/A324393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [A000120(d) does not divide n]
 * Range:     u8 n <= 9654, u16 n <= 9654, u32 n <= 9654, u64 n <= 9654,
 *            u128 n <= 9654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* popcount of a 64-bit unsigned integer */
static inline unsigned popcount_u64(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            unsigned pc = popcount_u64((uint64_t)d);
            if (pc != 0 && n % pc != 0)
                ++count;

            idx_t e = n / d;
            if (e != d) {
                unsigned pc2 = popcount_u64((uint64_t)e);
                if (pc2 != 0 && n % pc2 != 0)
                    ++count;
            }
        }
    }
    return count;
}
