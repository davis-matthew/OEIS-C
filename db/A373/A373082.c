/*
 * A373082  Number of runs of 0's in A014550(n).
 * https://oeis.org/A373082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of runs of 0's in the binary expansion of g = n
 *            XOR floor(n/2) (A014550), with a(0) = 1 (g = 0 is the single
 *            digit "0")
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t g = (uint64_t)(n ^ (n >> 1));
    if (g == 0)
        return (seq_t)1;
    /* Each zero-run is counted once, at the position where a 0 sits directly below a 1. */
    uint64_t t = (g >> 1) & ~g;
    seq_t c = 0;
    while (t) {
        t &= t - 1;
        ++c;
    }
    return c;
}
