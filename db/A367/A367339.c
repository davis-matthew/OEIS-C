/*
 * A367339  a(n) = A367338(n) - n, or -1 if A367338(n) = -1.
 * https://oeis.org/A367339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = A367338(n) - n, or -1 if A367338(n) = -1; i.e. the
 *            smallest 10*lsd(n)+y (y=1..9) with msd(n+10*lsd(n)+y) = y, else
 *            -1
 * Range:     i8 n <= 38, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9946 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    seq_t x = n_val % 10;
    for (seq_t y = 1; y <= 9; ++y) {
        seq_t d = 10 * x + y;
        seq_t t = n_val + d;
        while (t >= 10)
            t /= 10;
        if (t == y)
            return d;
    }
    return (seq_t)-1;
}
