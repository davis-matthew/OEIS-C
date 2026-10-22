/*
 * A384057  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is a 3-smooth number.
 * https://oeis.org/A384057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative, with a(p^e) = p^e if p ≤ 3, a(p^e) = p^e
 *            – 1 if p ≥ 5. Hence a(n) = ∏_{p^e‖n} f(p^e)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n ≥ 1.  For n == 0 the function is undefined, but we return 1 safely. */
    if (n == 0) return 1;

    /* Factor n into prime powers.  The helper `factor_u64` writes primes and
       exponents into the supplied arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t val = pow_uint(primes[i], exps[i]);  /* p^e */
        if (primes[i] <= 3u)
            prod *= val;                /* keep p^e itself */
        else
            prod *= (val - 1);           /* drop the prime factor 1 time */
    }
    return prod;
}
