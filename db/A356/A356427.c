/*
 * A356427  a(0) = 0, a(1) = 1; for n > 1, a(n) is the last step before
 *          reaching 0 of the iterations x -> x - gpf(x) starting at n, where
 *          gpf = A006530.
 * https://oeis.org/A356427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the prime that is subtracted in the last step of the
 *            iteration x -> x - gpf(x) starting at n (where gpf is the
 *            largest prime factor)
 * Range:     u8 n <= 77, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t x = (seq_t)n;
    while (1) {
        /* gpf works on 64–bit unsigned values */
        uint64_t ux = (uint64_t)x;
        uint64_t p  = gpf(ux);
        seq_t sp   = (seq_t)p;

        /* If x itself is prime, it is the last step. */
        if (x == sp)
            return x;

        /* Otherwise continue the iteration. */
        x -= sp;
    }
}
