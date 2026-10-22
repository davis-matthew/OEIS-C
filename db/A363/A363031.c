/*
 * A363031  a(n) = sigma(6*n+1). Sum of the divisors of 6*n+1, n >= 0.
 * https://oeis.org/A363031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(6*n+1)
 * Range:     u8 n <= 7, u16 n <= 7203, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sum of divisors of 6*n+1 for n ≥ 0. */

#include <stdint.h>
#include "../utils/pow_uint.h"

/* Return the sigma (sum of divisors) of 6*n+1. */
seq_t a(idx_t n)
{
    /* 6*n+1 fits in uint64_t since n is an idx_t (uint64_t). */
    uint64_t m      = (uint64_t)n * 6ULL + 1ULL;
    uint64_t temp   = m;
    seq_t sigma     = 1;

    /* Handle factor 2 separately. */
    if ((temp & 1ULL) == 0) {
        unsigned exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++exp;
        }
        seq_t part = pow_uint(2ULL, exp + 1);
        part -= 1;
        /* (2^(exp+1)-1)/(2-1) == 2^(exp+1)-1 */
        sigma *= part;
    }

    /* Test odd divisors up to sqrt(temp). */
    for (uint64_t i = 3; i * i <= temp; i += 2) {
        if (temp % i == 0) {
            unsigned exp = 0;
            while (temp % i == 0) {
                temp /= i;
                ++exp;
            }
            seq_t part = pow_uint(i, exp + 1);
            part -= 1;
            part /= (seq_t)(i - 1);    /* (i^(exp+1)-1)/(i-1) */
            sigma *= part;
        }
    }

    /* Remaining prime factor larger than sqrt(original m). */
    if (temp > 1) {
        sigma *= (seq_t)(temp + 1);   /* sigma(p) = 1 + p for prime p */
    }

    return sigma;
}
