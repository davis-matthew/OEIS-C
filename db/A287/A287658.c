/*
 * A287658  Positions of 0 in A287657; complement of A287659.
 * https://oeis.org/A287658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2n - A005614(n-1), with A005614(m) = floor((m+2)u) -
 *            floor((m+1)u), u = (sqrt(5)-1)/2; floor(k*u) = (isqrt(5k^2)-k)/2
 *            computed exactly
 * Range:     u8 n <= 6, u16 n <= 833, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9120 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A287658_isqrt(seq_t v)
{
    if (v < 2) return v;
    seq_t x = v;
    seq_t y = (x + v / x) / 2;
    while (y < x) {
        x = y;
        y = (x + v / x) / 2;
    }
    return x;
}

/* floor(k*(sqrt(5)-1)/2) */
static seq_t A287658_f(seq_t k)
{
    seq_t s = A287658_isqrt(5 * k * k);
    return (s - k) / 2;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* A005614(n-1) = f(n+1) - f(n) */
    seq_t d = A287658_f(N + 1) - A287658_f(N);
    return 2 * N - d;
}
