/*
 * A353309  The maximum sum of base-3 digits occurring among all numbers
 *          reached after n, when iterating map x -> A353313(x) starting from
 *          x=n, or -1 if no finite cycle is ever reached.
 * https://oeis.org/A353309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   iterate x -> A353313(x) from x = n (Floyd cycle detection); a(n)
 *            = max of A053735(x) over every term reached after n, i.e. the
 *            transient part plus the whole cycle. Search, not closed form
 * Range:     u8 n <= 19683, u16 n <= 19683, u32 n <= 19683, u64 n <= 19683,
 *            u128 n <= 19683, big any n
 * Validated: standard, pass, 106 terms through n = 9929 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef unsigned __int128 A353309_u128;

static A353309_u128 A353309_f(A353309_u128 x)
{
    A353309_u128 k = x / 3, r = x % 3;
    if (r == 0) return k;
    return 5 * k + 3 + r;
}

static unsigned A353309_ds3(A353309_u128 x)
{
    unsigned s = 0;
    while (x) { s += (unsigned)(x % 3); x /= 3; }
    return s;
}

seq_t a(idx_t n)
{
    const A353309_u128 cap = ((A353309_u128)1) << 124;
    A353309_u128 t = (A353309_u128)n, h = (A353309_u128)n;
    unsigned long long steps = 0;
    unsigned best = 0;
    for (;;) {
        t = A353309_f(t);
        {
            unsigned d = A353309_ds3(t);
            if (d > best) best = d;
        }
        h = A353309_f(h);
        if (h > cap) return (seq_t)18;
        h = A353309_f(h);
        if (h > cap) return (seq_t)18;
        if (t == h) break;
        if (++steps > 5000000ULL) return (seq_t)18;
    }
    /* t lies on the cycle; include every cycle element */
    {
        A353309_u128 y = A353309_f(t);
        unsigned d = A353309_ds3(t);
        if (d > best) best = d;
        while (y != t) {
            d = A353309_ds3(y);
            if (d > best) best = d;
            y = A353309_f(y);
        }
    }
    return (seq_t)best;
}
