/*
 * A372292  Numbers that occur more than once in the odd bisection of A371094.
 * https://oeis.org/A372292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   3v+1 = 4^i*w with i>=1 and w = 16 (mod 18) for odd i, w = 4 (mod
 *            18) for even i; a(n) = n-th v with at least 2 such i (search)
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 15532, u64 n <= 15532,
 *            u128 n <= 15532, big any n
 * Validated: standard, pass, 71 terms through n = 9574 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t v = 0;
    while (found < n) {
        ++v;
        seq_t N = 3 * v + 1;
        int cnt = 0;
        int i = 0;
        while ((N & 3) == 0) {
            N >>= 2;
            ++i;
            seq_t r = N % 18;
            if (i & 1) {
                if (r == 16) ++cnt;
            } else {
                if (r == 4) ++cnt;
            }
        }
        if (cnt >= 2) ++found;
    }
    return v;
}
