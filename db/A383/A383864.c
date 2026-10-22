/*
 * A383864  The sum of divisors d of n having the property that for every
 *          prime p dividing n the p-adic valuation of d is either 0 or a
 *          unitary divisor of the p-adic valuation of n.
 * https://oeis.org/A383864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e∥n} ( 1 + Σ_{d|e , gcd(d,e/d)=1} p^d )
 *            (multiplicative definition for A383864)
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd_int, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utilities used by this function. */
#include "../utils/factor_u64.h"
#include "../utils/gcd_int.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Special case: 1 has no prime factors, a(1)=1. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factor n into its prime powers. 64 entries are more than enough
       for 64‑bit integers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute a(p^e): start with the 1 corresponding to d=0. */
        seq_t sum = (seq_t)1;
        for (uint64_t d = 1; d <= e; ++d) {
            if (e % d == 0) {
                uint64_t other = e / d;
                /* gcd(d, e/d) must be 1. */
                if (gcd_int((int)d, (int)other) == 1)
                    sum += pow_uint(p, (idx_t)d);
            }
        }
        result *= sum;
    }
    return result;
}
