/*
 * A274204  Numbers n such that there is no cubefree number that is the sum of
 *          two nonzero squares in exactly n ways.
 * https://oeis.org/A274204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A274204:
 * a(n) is the n-th positive integer that is NOT of the form 2^c * 3^b
 * (c,b ≥ 0) nor of the form (3^b ± 1)/2 (b ≥ 0).
 * These forms are exactly the possible counts of unordered representations
 * of cubefree numbers as the sum of two nonzero squares.
 */

#include <stdbool.h>

static bool is_in_set(seq_t k)
{
    if (k == 0) return false;

    /* check form 2^c * 3^b */
    seq_t t = k;
    while ((t & (seq_t)1) == 0)   /* divide by 2 while even */
        t >>= 1;
    while (t % (seq_t)3 == 0)
        t /= 3;
    if (t == 1) return true;

    /* check form (3^b + 1)/2  -> 2k + 1 must be a power of 3 (b ≥ 0) */
    seq_t y = (seq_t)2 * k + 1;
    while (y % (seq_t)3 == 0)
        y /= 3;
    if (y == 1) return true;

    /* check form (3^b - 1)/2  -> 2k - 1 must be a power of 3 (b ≥ 1) */
    seq_t z = (seq_t)2 * k - 1;
    if (z > 0) {
        while (z % (seq_t)3 == 0)
            z /= 3;
        if (z == 1) return true;
    }

    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, but harmless */

    idx_t missing = 0;
    idx_t val = 1;

    while (true) {
        if (!is_in_set((seq_t)val)) {
            ++missing;
            if (missing == n)
                return (seq_t)val;
        }
        ++val;
    }
}
