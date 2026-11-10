/*
 * A269529  An analog of the Golay-Rudin-Shapiro sequence (A020985) in base -2
 *          (see comments).
 * https://oeis.org/A269529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = 1 if A269027(n) = A269528(n), else -1. A269027: a(0)=0,
 *            a(1)=1, a(4k)=a(k), a(4k+1)=1-a(k), a(4k+2)=1-a(k+1),
 *            a(4k+3)=a(k+1). A269528: parity of the number of runs of 1's in
 *            the base -2 representation of n
 * Range:     i8 n <= 1999, i16 n <= 1999, i32 n <= 1999, i64 n <= 1999,
 *            i128 n <= 1999, big any n
 * Validated: standard, pass, 92 terms through n = 1773 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned A269529_tm(idx_t n)
{
    if (n <= 1)
        return (unsigned)n;
    idx_t k = n >> 2;
    switch (n & 3) {
        case 0: return A269529_tm(k);
        case 1: return A269529_tm(k) ^ 1u;
        case 2: return A269529_tm(k + 1) ^ 1u;
        default: return A269529_tm(k + 1);
    }
}

static unsigned A269529_runs(idx_t n)
{
    if (n == 0) return 0;
    __int128 x = (__int128)n;
    unsigned parity = 0;
    bool prev1 = false;
    while (x != 0) {
        unsigned bit = (unsigned)(x & 1);
        if (bit) {
            if (!prev1) {
                parity ^= 1u;
                prev1 = true;
            }
        } else {
            prev1 = false;
        }
        x = (x - bit) / (-2);
    }
    return parity;
}

seq_t a(idx_t n)
{
    return (A269529_tm(n) == A269529_runs(n)) ? (seq_t)1 : (seq_t)(-1);
}
