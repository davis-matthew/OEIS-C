/*
 * A306944  a(n) = number of iterations of x -> A306938(x) required to reach 1
 *          when started at n, or -1 if the trajectory of n never reaches 1.
 * https://oeis.org/A306944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = number of iterations of x -> A306938(x) to reach 1 from
 *            n, else -1; computed by direct iteration, returning -1 when two
 *            consecutive non-multiples of 3 occur (te Riele: then it grows
 *            forever)
 * Range:     i8 n <= 6, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9782 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A306944_isqrt(seq_t v)
{
    if (v < 2) return v;
    seq_t r = v;
    seq_t y = (r + v / r) / 2;
    while (y < r) {
        r = y;
        y = (r + v / r) / 2;
    }
    return r;
}

static seq_t A306944_step(seq_t x)
{
    if (x % 3 == 0) return x / 3;
    return A306944_isqrt(x * x * 3);
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    long long steps = 0;
    while (x != 1) {
        seq_t y = A306944_step(x);
        if (x % 3 != 0 && y % 3 != 0) return (seq_t)(-1);
        x = y;
        steps++;
        if (steps > 1000000) return (seq_t)(-1);
    }
    return (seq_t)steps;
}
