/*
 * A374538  a(n) is the sum of the squares of the unitary divisors of n that
 *          are exponentially odd numbers (A268335).
 * https://oeis.org/A374538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^(2e) + 1 if e is odd, otherwise 1)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 1 the product over
       prime powers is empty, so we return 1. */
    if (n == (idx_t)0) return (seq_t)0;   /* undefined; guard for safety */
    if (n == (idx_t)1) return (seq_t)1;

    /* Factor n into prime powers.  For n ≤ 10000 we never have more
       than 6 distinct prime factors, so 16 is plenty. */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        if (e & 1U) /* e is odd */
        {
            seq_t mult = big_pow((seq_t)primes[i], (seq_t)(2 * e));
            result = result * (mult + (seq_t)1);
        }
        /* if e is even the factor is 1; nothing to do */
    }

    return result;
}
