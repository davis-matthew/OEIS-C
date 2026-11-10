/*
 * A249670  a(n) = A017665(n)*A017666(n).
 * https://oeis.org/A249670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A017665(n)*A017666(n) = (sigma(n)/g) * (n/g), where g =
 *            gcd(n, sigma(n))
 * Range:     u8 n <= 15, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9452 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A249670_sigma(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t s = A249670_sigma(n);
    seq_t g = gcd(nn, s);
    return (s / g) * (nn / g);
}
