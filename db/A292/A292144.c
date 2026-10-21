/*
 * A292144  a(n) is the greatest k < n such that k*n is square.
 * https://oeis.org/A292144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a * (d-1)^2, where n = a * d^2 with a squarefree. If d =
 *            1 (i.e., n is squarefree) then a(n) = 0
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* local copy of n for factorisation */
    uint64_t m = (uint64_t)n;
    seq_t a_part = 1;   /* squarefree part */
    seq_t d_part = 1;   /* square root part */

    /* factor 2 separately */
    if (m % 2 == 0) {
        int exp = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++exp;
        }
        if (exp & 1) a_part *= (seq_t)2;
        int half = exp >> 1;
        for (int i = 0; i < half; ++i) d_part *= (seq_t)2;
    }

    /* odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            if (exp & 1) a_part *= (seq_t)p;
            int half = exp >> 1;
            for (int i = 0; i < half; ++i) d_part *= (seq_t)p;
        }
    }

    /* remaining prime factor > sqrt(m) */
    if (m > 1) {
        /* exponent is 1 (odd) */
        a_part *= (seq_t)m;
        /* half exponent is 0, d_part unchanged */
    }

    /* if d_part == 1 (i.e., n was squarefree) */
    if (d_part == 1) return 0;

    seq_t dm = d_part - 1;
    return a_part * dm * dm;
}
