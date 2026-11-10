/*
 * A277699  Main diagonal of A277320: a(n) = A048720(n, A065621(n)).
 * https://oeis.org/A277699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A048720(n, A065621(n)), carryless (GF(2)[x]) product of n
 *            and A065621(n), where A065621(n) = n XOR ((n - lowbit(n)) << 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 71 terms through n = 8034 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t low = (seq_t)(n & (~n + 1));
    seq_t r = nt ^ ((nt - low) << 1);   /* A065621(n) */
    seq_t result = 0;
    idx_t j = n;
    while (j) {
        if (j & 1)
            result ^= r;
        r <<= 1;
        j >>= 1;
    }
    return result;
}
