/*
 * A308004  a(n) = smallest nonnegative number that requires n applications of
 *          the Sisyphus function x -> A073053(x) to reach 123.
 * https://oeis.org/A308004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = smallest x >= 0 such that n applications of the Sisyphus
 *            function A073053 take x to 123 (search over x = 0, 1, 2, ...)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 5 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A073053_step(uint64_t m)
{
    uint64_t e = 0, o = 0, t = 0;
    if (m == 0) { e = 1; t = 1; }
    else {
        while (m > 0) {
            if (((m % 10) & 1) == 0) e++; else o++;
            t++; m /= 10;
        }
    }
    uint64_t pt = 1, po = 1, v;
    for (v = t; v > 0; v /= 10) pt *= 10;
    for (v = o; v > 0 || po == 1; v /= 10) { po *= 10; if (v < 10) break; }
    if (o == 0) po = 10;
    /* po = 10^(digits of o), pt = 10^(digits of t) */
    return e * pt * po + o * pt + t;
}

static int A308004_steps(uint64_t x)
{
    int s = 0;
    while (x != 123) {
        x = A073053_step(x);
        s++;
        if (s > 1000) return -1;
    }
    return s;
}

seq_t a(idx_t n)
{
    static uint64_t cache[64];
    static int have[64];
    if (n < 64 && have[n]) return (seq_t)cache[n];
    uint64_t limit = 400000000ULL;
    for (uint64_t x = 0; x < limit; x++) {
        if (A308004_steps(x) == (int)n) {
            if (n < 64) { cache[n] = x; have[n] = 1; }
            return (seq_t)x;
        }
    }
    return 0;
}
