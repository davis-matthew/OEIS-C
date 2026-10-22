/*
 * A360523  a(n) = Sum_{d|n} mu(rad(d)) * delta_d(n/d), where rad(n) =
 *          A007947(n) and delta_d(n) is the greatest divisor of n that is
 *          relatively prime to d.
 * https://oeis.org/A360523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e||n of (p^e - e)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Multiplicative definition) */

#include "../utils/pow_uint.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, just for safety */
    uint64_t m = n;
    seq_t result = 1;

    /* Factor 2 separately */
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) {
            m /= 2;
            ++e;
        }
        seq_t term = pow_uint(2, e) - (seq_t)e;
        result *= term;
    }

    /* Factor odd primes */
    for (uint64_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) {
            int e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            seq_t term = pow_uint(i, e) - (seq_t)e;
            result *= term;
        }
    }

    /* Remaining prime factor, if any */
    if (m > 1) {
        seq_t term = pow_uint(m, 1) - (seq_t)1;
        result *= term;
    }

    return result;
}
