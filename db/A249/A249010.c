/*
 * A249010  Expansion of (P(q) - 3*P(q^2) - 5*P(q^5) + 15*P(q^10)) / 8 in
 *          powers of q where P() is a Ramanujan Eisenstein series.
 * https://oeis.org/A249010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3 * b(n) for n > 0, a(0) = 1 where b is multiplicative
 *            with: b(2^e) = 2 - 2^e b(5^e) = 1 b(p^e) = 1 + p + p^2 + ... +
 *            p^e for all other primes p
 * Range:     i8 n <= 32, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t b = (seq_t)1;
    idx_t temp = n;

    /* factor powers of 2 */
    idx_t e2 = 0;
    while ((temp & 1ULL) == 0) {
        e2++;
        temp >>= 1;
    }
    if (e2 > 0) {
        seq_t pow2 = (seq_t)1;
        for (idx_t i = 0; i < e2; i++)
            pow2 *= (seq_t)2;
        seq_t b2 = (seq_t)2 - pow2; /* 2 - 2^e2 */
        b *= b2;
    }

    /* factor remaining odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            if (p == 5) /* b(5^e) = 1, factor 1 has no effect */
                continue;

            /* compute 1 + p + ... + p^e */
            seq_t sum = (seq_t)1;
            seq_t pk = (seq_t)1;
            for (idx_t i = 0; i < e; i++) {
                pk *= (seq_t)p;
                sum += pk;
            }
            b *= sum;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1) {
        if (temp != 5) {
            seq_t sum = (seq_t)1 + (seq_t)temp;
            b *= sum;
        }
        /* else temp == 5: factor 1, nothing to do */
    }

    return (seq_t)(-3) * b;
}
