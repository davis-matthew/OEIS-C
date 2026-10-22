/*
 * A381954  The maximum exponent in the prime factorization of n that is
 *          coprime to n, or 0 if no such exponent exists.
 * https://oeis.org/A381954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{ e | e is the exponent of a prime in the
 *            factorization of n and gcd(e,n) = 1 }; 0 if no such exponent
 *            exists
 * Range:     u8 n <= 8045, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include <stdint.h>          /* for uint64_t */

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* factor_u64 writes the prime factors and their exponents into these
       arrays and returns the number of distinct prime factors in *cnt. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 uses uint64_t as the input type. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t exp = (seq_t)exps[i];
        /* gcd expects arguments of type seq_t. */
        if (gcd(exp, (seq_t)n) == 1 && exp > best) {
            best = exp;
        }
    }

    return best;
}
