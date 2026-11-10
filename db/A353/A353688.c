/*
 * A353688  a(n) = n / A098988(n).
 * https://oeis.org/A353688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / A098988(n) = gcd( Sum_{d|n} (-1)^(d+1) * (n/d), n )
 *            for n >= 1; a(0) = 0
 * Range:     u8 n <= 3275, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9823 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Track positive and negative parts modulo n to avoid overflow. */
    idx_t pos = 0, neg = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i, d2 = n / i;
            idx_t t1 = n / d1;
            if (d1 & 1) pos = (pos + t1) % n; else neg = (neg + t1) % n;
            if (d1 != d2) {
                idx_t t2 = n / d2;
                if (d2 & 1) pos = (pos + t2) % n; else neg = (neg + t2) % n;
            }
        }
    }
    idx_t s = (pos + n - neg) % n;   /* sum mod n */

    idx_t x = s, y = n;
    while (x != 0) {
        idx_t t = y % x;
        y = x;
        x = t;
    }
    return (seq_t)y;                 /* gcd(sum, n); gcd(0, n) = n */
}
