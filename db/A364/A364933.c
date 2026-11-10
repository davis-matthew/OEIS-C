/*
 * A364933  a(n) = Sum_{k=1..n} A191898(n,k)*[A191904(n,k) = A191898(n,k)].
 * https://oeis.org/A364933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A057859(n) - A008472(n) - 1 = phi(n) + omega(n) -
 *            sopfr_distinct(n) - 1 (conjectured in OEIS; checked against
 *            listed terms)
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m < 1) return (seq_t)0;
    seq_t phi = (seq_t)m;
    seq_t om = (seq_t)0;
    seq_t sp = (seq_t)0;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi -= phi / (seq_t)p;
            om += 1;
            sp += (seq_t)p;
        }
    }
    if (m > 1) {
        phi -= phi / (seq_t)m;
        om += 1;
        sp += (seq_t)m;
    }
    return phi + om - sp - 1;
}
