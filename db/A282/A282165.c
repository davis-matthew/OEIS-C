/*
 * A282165  Partial products of A061017.
 * https://oeis.org/A282165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} A061017(k), where A061017 lists each m
 *            exactly d(m) times, so a(n) = Product_{m} m^(number of copies of
 *            m among the first n terms)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 100 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A282165_tau(idx_t x)
{
    idx_t cnt = 1;
    idx_t t = x;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            cnt *= (e + 1);
        }
    }
    if (t > 1)
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t remaining = n;
    for (idx_t m = 1; remaining > 0; ++m) {
        idx_t c = A282165_tau(m);
        if (c > remaining)
            c = remaining;
        for (idx_t i = 0; i < c; ++i)
            result = result * (seq_t)m;
        remaining -= c;
    }
    return result;
}
