/*
 * A342918  a(n) = (1+n) / A143771(n).
 * https://oeis.org/A342918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) / A143771(n), where A143771(n) = gcd of (k + n/k)
 *            over all divisors k of n
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 9213 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A143771_a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    seq_t g = 0;

    for (idx_t i = 1; i <= n / i; ++i) {
        if ((n % i) == 0) {
            seq_t val = (seq_t)i + (seq_t)(n / i);
            if (g == 0) {
                g = val;
            } else {
                g = gcd(g, val);
            }
        }
    }

    return g;
}

seq_t a(idx_t n)
{
    seq_t g = A143771_a(n);
    if (g == 0) {
        return 0;
    }
    return ((seq_t)n + 1) / g;
}
