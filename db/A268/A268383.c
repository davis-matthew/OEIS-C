/*
 * A268383  Least monotonic left inverse of A268412.
 * https://oeis.org/A268383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = a(n-1) + [A268411(n) == 0 and n >= 1], i.e.
 *            count of m in 1..n with an even number of runs of 1's in binary
 *            (direct count)
 * Range:     u8 n <= 512, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 97 terms through n = 9887 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A268383_runs_parity(idx_t x)
{
    unsigned parity = 0;
    unsigned prev = 0;
    while (x) {
        unsigned bit = (unsigned)(x & 1);
        if (bit && !prev)
            parity ^= 1;
        prev = bit;
        x >>= 1;
    }
    return parity;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t m;
    for (m = 1; m <= n; m++) {
        if (A268383_runs_parity(m) == 0)
            count++;
    }
    return count;
}
