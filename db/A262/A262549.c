/*
 * A262549  Read A006530 digit-by-digit.
 * https://oeis.org/A262549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th digit of the concatenation of A006530(1),
 *            A006530(2), ...; computed by scanning A006530 terms (greatest
 *            prime factor, a(1)=1) and counting digits
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Validated: partial, pass, 130 terms through n = 130 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A006530_gpf(uint64_t m)
{
    if (m == 1)
        return 1;
    uint64_t g = 1;
    while (m % 2 == 0) {
        g = 2;
        m /= 2;
    }
    for (uint64_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) {
            g = i;
            m /= i;
        }
    }
    if (m > 1)
        g = m;
    return g;
}

seq_t a(idx_t n)
{
    uint64_t pos = 0;
    for (uint64_t m = 1;; m++) {
        uint64_t g = A006530_gpf(m);
        unsigned d = 0;
        uint64_t t = g;
        while (t) {
            d++;
            t /= 10;
        }
        if (pos + d >= (uint64_t)n) {
            /* digit index from the left (0-based) */
            unsigned k = (unsigned)((uint64_t)n - pos - 1);
            unsigned drop = d - 1 - k;
            while (drop--)
                g /= 10;
            return (seq_t)(g % 10);
        }
        pos += d;
    }
}
