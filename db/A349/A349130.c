/*
 * A349130  a(n) = Sum_{d|n} d * A003958(n/d), where A003958 is fully
 *          multiplicative with a(p) = (p-1).
 * https://oeis.org/A349130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * A003958(n/d)
 * Range:     u8 n <= 59, u16 n <= 4289, u32 n <= 9326, u64 n <= 9326,
 *            u128 n <= 9326, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    idx_t m = n;
    seq_t result = (seq_t)1;

    for (idx_t p = (idx_t)2; p <= m / p; ++p) {
        if (m % p == (idx_t)0) {
            size_t e = (size_t)0;
            while (m % p == (idx_t)0) {
                m /= p;
                ++e;
            }

            /* compute p^(e+1) */
            seq_t pe1 = (seq_t)1;
            for (size_t i = (size_t)0; i < e + (size_t)1; ++i)
                pe1 *= (seq_t)p;

            /* compute (p-1)^(e+1) */
            seq_t pm1e1 = (seq_t)1;
            for (size_t i = (size_t)0; i < e + (size_t)1; ++i)
                pm1e1 *= (seq_t)(p - (idx_t)1);

            seq_t term = pe1 - pm1e1;
            result *= term;
        }
    }

    /* if a prime factor remains */
    if (m > (idx_t)1) {
        idx_t p = m;
        seq_t pe1 = (seq_t)p * (seq_t)p;               /* p^(1+1) */
        seq_t pm1e1 = (seq_t)(p - (idx_t)1) * (seq_t)(p - (idx_t)1); /* (p-1)^(1+1) */
        seq_t term = pe1 - pm1e1;
        result *= term;
    }

    return result;
}
