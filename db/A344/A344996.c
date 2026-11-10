/*
 * A344996  a(n) = A048250(n) * A051709(n).
 * https://oeis.org/A344996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048250(n) * A051709(n), where A048250(n) =
 *            prod_{p|n}(p+1) and A051709(n) = sigma(n) + phi(n) - 2n
 * Range:     u8 n <= 27, u16 n <= 209, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 93 terms through n = 9959 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A344996_a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t sigma = 1, phi = 1, rad = 1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t s = 1, pk = 1, ph = 1;
            m /= p;
            while (1) {
                pk *= (seq_t)p;
                s += pk;
                if (m % p == 0) {
                    m /= p;
                    ph *= (seq_t)p;
                } else break;
            }
            sigma *= s;
            phi *= ph * ((seq_t)p - 1);
            rad *= (seq_t)p + 1;
        }
    }
    if (m > 1) {
        sigma *= (seq_t)m + 1;
        phi *= (seq_t)m - 1;
        rad *= (seq_t)m + 1;
    }

    seq_t t = sigma + phi;
    t -= (seq_t)n * 2;
    return rad * t;
}

seq_t a(idx_t n)
{
    return A344996_a(n);
}
