/*
 * A391186  The largest divisor of n that is a number of the form x^2 + 2*y^2
 *          (A002479).
 * https://oeis.org/A391186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A391186
 * Formula: a(n) = largest divisor of n that is of the form x^2 + 2*y^2.
 * For n = ∏ p_i^e_i, a(n) = ∏ p_i^e'_i,
 *   where e'_i = e_i          if p_i ≡ 1,3 (mod 8) or p_i = 2
 *                = 2*⌊e_i/2⌋ if p_i ≡ 5,7 (mod 8).
 *
 * Implementation: factor n, adjust exponents according to the rule,
 * then multiply the primes to the appropriate power.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* for factor_u64() */

static seq_t pow_uint(uint64_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp > 0) {
        if (exp & 1) {
            result = result * b;
        }
        b = b * b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Factor n into primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t eprime;

        /* Adjust exponent according to p mod 8 */
        if ((p % 8) == 5 || (p % 8) == 7) {
            if (e & 1)   /* e is odd */
                eprime = e - 1;
            else
                eprime = e;
        } else {
            eprime = e;
        }

        result = result * pow_uint(p, eprime);
    }

    return result;
}
