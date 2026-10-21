/*
 * A284341  Sum of the divisors of n that are not divisible by 8.
 * https://oeis.org/A284341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d|n, 8∤d} d (OEIS A284341)
 * Range:     u8 n <= 16, u16 n <= 316, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* small power function suitable for seq_t types */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp--)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* Undefined for n=0, but harmless */

    if (n == 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t res = (seq_t)1;

    /* factor 2 separately */
    unsigned int e2 = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        e2++;
    }
    if (e2) {
        seq_t mult;
        if (e2 >= 3) {
            mult = (seq_t)7; /* sigma(2^e) for e>=3 */
        } else {
            /* compute sigma(2^e) = 1 + 2 + ... + 2^e */
            seq_t cur = (seq_t)1;
            mult = (seq_t)1;
            for (unsigned int i = 0; i < e2; i++) {
                cur *= (seq_t)2;
                mult += cur;
            }
        }
        res *= mult;
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t p_pow = pow_seq((seq_t)p, (idx_t)(e + 1));
            seq_t mult = (p_pow - (seq_t)1) / ((seq_t)p - (seq_t)1);
            res *= mult;
        }
    }

    /* remaining prime factor > sqrt(m) */
    if (m > 1) {
        seq_t mult = ((pow_seq((seq_t)m, (idx_t)2) - (seq_t)1) /
                      ((seq_t)m - (seq_t)1));
        res *= mult;
    }

    return res;
}
