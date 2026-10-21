/*
 * A275966  a(n) is the real part of -I*Sum_{d|n}(mobius(d)*I^(n/d)),
 *          I=sqrt(-1), mobius(n) is A008683.
 * https://oeis.org/A275966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = multiplicative product over prime powers; For prime
 *            exponent e: if p == 2: a(2^1) = -1, a(2^e>=2) = 0 else if p % 4
 *            == 1: a(p^e) = 0 else (p % 4 == 3): a(p^e) = (-1)^e * 2 (i.e., 2
 *            if e even, -2 if e odd)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;   /* use 64‑bit for temporary factors */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }

            seq_t val;
            if (p == 2) {
                val = (exp == 1 ? (seq_t)-1 : (seq_t)0);
            } else if (p % 4 == 1) {
                val = (seq_t)0;
            } else {            /* p % 4 == 3 */
                val = (exp % 2 == 0 ? (seq_t)2 : (seq_t)-2);
            }

            if (val == 0) return (seq_t)0;
            result *= val;
        }
    }

    /* m is now a prime > sqrt(original n) if greater than 1 */
    if (m > 1) {
        uint64_t p = m;  /* exponent is 1 */
        seq_t val;
        if (p == 2) {
            val = (seq_t)-1;
        } else if (p % 4 == 1) {
            val = (seq_t)0;
        } else {
            val = (seq_t)-2;    /* odd exponent */
        }

        if (val == 0) return (seq_t)0;
        result *= val;
    }

    return result;
}
