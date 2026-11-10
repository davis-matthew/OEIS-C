/*
 * A383276  Numbers of the form A034444(k) * k.
 * https://oeis.org/A383276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th m (increasing) with A007814(m) = A005087(m) or
 *            A007814(m) > A005087(m)+1; found by a search over m
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9972 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A383276_ok(idx_t m)
{
    idx_t v = 0, w = 0;
    while ((m & 1) == 0) { m >>= 1; v++; }
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            w++;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) w++;
    return v == w || v > w + 1;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n) {
        m++;
        if (A383276_ok(m)) count++;
    }
    return (seq_t)m;
}
