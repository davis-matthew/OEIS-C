/*
 * A360999  Number of tilings of an n X 2 rectangle by integer-sided
 *          rectangular pieces that cannot be rearranged to produce a
 *          different tiling of the rectangle (including rotations and
 *          reflections of the original tiling).
 * https://oeis.org/A360999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000005(n) - 1 - [n even]
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the Number of tilings of an n × 2 rectangle by integer‑sided rectangular pieces
   that cannot be rearranged to produce a different tiling, as defined by OEIS A360999. */
seq_t a(idx_t n) {
    if (n == (idx_t)1) {
        return (seq_t)2;
    }

    uint64_t m = n;          /* working copy of n */
    uint64_t d64 = 1;        /* number of divisors, fits in 64‑bit for n <= 10000 */

    /* factor n and count exponents */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            d64 *= (exp + 1);
        }
    }
    /* if something remains, it's a prime factor > sqrt(original n) */
    if (m > 1) {
        d64 *= 2ULL;
    }

    seq_t d = (seq_t)d64;                /* convert divisor count to seq_t */
    seq_t val = (seq_t)2 * d - (seq_t)1; /* 2*d - 1  */
    if ((n & (idx_t)1) == 0) {           /* subtract additional 1 if n is even */
        val -= (seq_t)1;
    }
    return val;
}
