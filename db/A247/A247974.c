/*
 * A247974  Numbers k such that A247973(k+1) = A247973(k).
 * https://oeis.org/A247974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th number k with A247973(k+1) = A247973(k), where
 *            A247973(n) is the least j with Pi - (4j+2)/w(j)^2 < 1/n,
 *            w(1)=3/2, w(j)=w(j-1)*(2j+1)/(2j). Computed by a scan over k
 * Range:     u8 n <= 55, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static const long double A247974_PI = 3.14159265358979323846264338327950288419716939937510L;

static long double A247974_D(idx_t k, long double w)
{
    return A247974_PI - (4.0L * (long double)k + 2.0L) / (w * w);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t k = 1;
    long double w = 1.5L;
    long double D = A247974_D(k, w);
    idx_t count = 0;
    idx_t m = 1;

    for (;; ++m) {
        long double inv = 1.0L / (long double)m;
        while (D >= inv) {
            ++k;
            w *= (2.0L * (long double)k + 1.0L) / (2.0L * (long double)k);
            D = A247974_D(k, w);
        }
        /* A247973(m) = k; check whether A247973(m+1) is also k */
        if (D < 1.0L / ((long double)m + 1.0L)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
