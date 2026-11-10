/*
 * A276614  The infinite trunk of greedy cubes beanstalk with reversed
 *          subsections.
 * https://oeis.org/A276614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 7; for n > 1, with g = A261225(a(n-1)): if g+1
 *            is not a cube, a(n) = g, otherwise a(n) = A000578(2 +
 *            A048766(a(n-1))) - 1 (iterated from a(1) = 7; A261225(x) = x -
 *            number of cubes in the greedy cube representation of x)
 * Range:     u8 n <= 33, u16 n <= 7745, u32 n <= 10236, u64 n <= 10236,
 *            u128 n <= 10236, big any n
 * Validated: standard, pass, 86 terms through n = 8435 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A276614_icbrt(seq_t x)
{
    if (x == 0) return 0;
    seq_t hi = 1;
    while (hi <= x / hi / hi) hi *= 2;
    seq_t lo = hi / 2;
    /* cbrt(x) in [lo, hi) */
    while (lo < hi - 1) {
        seq_t mid = lo + (hi - lo) / 2;
        if (mid <= x / mid / mid) lo = mid;
        else hi = mid;
    }
    return lo;
}

static seq_t A276614_g(seq_t x)
{
    seq_t orig = x, count = 0;
    while (x > 0) {
        seq_t c = A276614_icbrt(x);
        x -= c * c * c;
        count++;
    }
    return orig - count;
}

static seq_t A276614_next(seq_t v)
{
    seq_t g = A276614_g(v);
    seq_t t = g + 1;
    seq_t r = A276614_icbrt(t);
    if (r * r * r == t) {
        seq_t q = A276614_icbrt(v) + 2;
        return q * q * q - 1;
    }
    return g;
}

seq_t a(idx_t n)
{
    static idx_t cache_n = 0;
    static seq_t cache_v;
    if (n == 0) return 0;
    if (cache_n == 0 || n < cache_n) {
        cache_n = 1;
        cache_v = 7;
    }
    while (cache_n < n) {
        cache_v = A276614_next(cache_v);
        cache_n++;
    }
    return cache_v;
}
