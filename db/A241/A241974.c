/*
 * A241974  a(n) is the limit of the sequence given by A241083^[i](n), where
 *          f^[i] means iterate f i times, or 0 if the sequence diverges.
 * https://oeis.org/A241974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = limit of iterating A241083(m) = lcm(m, floor(sqrt(m)))
 *            starting at m = n; the code iterates until a fixed point, or
 *            returns 0 if the values grow without bound (more than 4400 bits,
 *            which is past 10^1310)
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-07)
 * Helpers:   bit_length, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/bit_length.h"

/* SHARED_HELPER: integer square root floor(sqrt(x)) over seq_t */
static seq_t A241974_isqrt(seq_t m)
{
    if (m < 2) return m;
    unsigned bl = bit_length(m);
    seq_t x = (seq_t)1;
    x <<= (bl + 1) / 2;              /* x >= sqrt(m) */
    for (;;) {
        seq_t y = (x + m / x) / 2;
        if (y >= x) break;
        x = y;
    }
    while (x * x > m) x--;
    return x;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (n == 0) return (seq_t)0;
    for (int it = 0; it < 100000; it++) {
        seq_t s = A241974_isqrt(m);
        seq_t g = gcd(m, s);
        seq_t q = m / g;
        seq_t r = q * s;
        if (s != 0 && r / s != q) return (seq_t)0; /* overflow in fixed width */
        if (r < m) return (seq_t)0;                /* wrapped */
        if (r == m) return m;
        m = r;
        if (bit_length(m) > 4400) return (seq_t)0;
    }
    return (seq_t)0;
}
