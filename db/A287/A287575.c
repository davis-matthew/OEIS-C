/*
 * A287575  Positions of 3 in A287571.
 * https://oeis.org/A287575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(n-1) + ((1 - d) mod 4) + 1, where d = A287571(n) is
 *            the n-th letter (1-based) of the fixed point of 0->0312,
 *            1->3120, 2->1203, 3->2031. Each image block of length 4 holds
 *            exactly one 3, so the n-th 3 sits in block n-1
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9892 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Letter at 0-based position i of the fixed point; image of d at k is base[(k+d)%4]. */
static unsigned A287571_digit(uint64_t i)
{
    static const unsigned base[4] = {0, 3, 1, 2};
    unsigned d = 0;
    for (int s = 62; s >= 0; s -= 2) {
        unsigned q = (unsigned)((i >> s) & 3u);
        d = base[(q + d) & 3u];
    }
    return d;
}

seq_t a(idx_t n)
{
    uint64_t j = (uint64_t)n - 1;
    unsigned d = A287571_digit(j);
    unsigned k = (1u + 4u - d) & 3u;
    seq_t r = (seq_t)j * 4 + (seq_t)k + 1;
    return r;
}
