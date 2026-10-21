/*
 * A283078  a(n) = sigma(7*n).
 * https://oeis.org/A283078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(7*n)
 * Range:     u8 n <= 17, u16 n <= 2519, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the sum of divisors of 7*n using multiplicativity */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* out-of-range, not used */

    idx_t m = 7 * n;   /* 7*n fits into idx_t for the expected input range */
    seq_t result = 1;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* sum of powers: 1 + p + p^2 + ... + p^exp */
            seq_t term = 1;
            seq_t power = 1;
            seq_t p_seq = (seq_t)p;
            for (idx_t i = 0; i < exp; ++i) {
                power *= p_seq;
                term += power;
            }
            result *= term;
        }
    }
    if (m > 1) {   /* remaining prime factor */
        seq_t prime = (seq_t)m;
        result *= (seq_t)1 + prime;
    }
    return result;
}
