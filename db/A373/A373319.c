/*
 * A373319  Denominator of the asymptotic density of numbers that are
 *          unitarily divided by n.
 * https://oeis.org/A373319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of φ(n)/n^2 = n^2 / gcd( φ(n), n^2 )
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not required, but safe */

    uint64_t nn = (uint64_t)n;     /* temporary 64‑bit value for factorisation */
    seq_t phi  = (seq_t)nn;        /* φ(n) will be computed in seq_t */
    uint64_t m = nn;

    /* factor 2 separately */
    if (m % 2 == 0) {
        phi -= phi / (seq_t)2;
        while (m % 2 == 0) m /= 2;
    }

    /* odd factors up to sqrt(m) */
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) {
            phi -= phi / (seq_t)d;
            while (m % d == 0) m /= d;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        phi -= phi / (seq_t)m;
    }

    /* compute n^2 and reduce the fraction φ(n)/n^2 */
    seq_t denom = (seq_t)nn * (seq_t)nn;
    seq_t g = gcd(phi, denom);
    return denom / g;
}
