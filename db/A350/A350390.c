/*
 * A350390  a(n) is the largest exponentially odd divisor of n.
 * https://oeis.org/A350390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = multiplicative with a(p^e) = p^e if e is odd,
 *               p^(e-1) otherwise.
 *
 * This implementation factors n by trial division, applies the
 * rule for each prime power, and multiplies the contributions.
 * It works for any n ≥ 1 where n fits in idx_t (uint64_t).
 */

#include <stdint.h>
#include <stdbool.h>

/* Utility functions for arbitrary-precision arithmetic.
 * These are provided by the build environment.
 */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* sequence is not defined for n=0, return 0 */
        return (seq_t)0;
    }

    uint64_t temp = (uint64_t)n;          /* work with uint64_t for factorisation */
    seq_t result = (seq_t)1;              /* multiplicative identity */

    /* factor 2 separately */
    idx_t cnt = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        cnt++;
    }
    if (cnt) {
        idx_t exp = (cnt & 1) ? cnt : cnt - 1;   /* odd exponent? */
        if (exp) {
            result = seq_mul(result, seq_pow((seq_t)2, exp));
        }
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p != 0)
            continue;
        cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            cnt++;
        }
        idx_t exp = (cnt & 1) ? cnt : cnt - 1;
        if (exp) {
            result = seq_mul(result, seq_pow((seq_t)p, exp));
        }
    }

    /* if a prime factor remains >1 */
    if (temp > 1) {
        /* exponent is 1 (odd), contribution is the prime itself */
        result = seq_mul(result, (seq_t)temp);
    }

    return result;
}
