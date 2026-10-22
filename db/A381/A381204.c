/*
 * A381204  a(n) is the gcd of the elements of the set of bases and exponents
 *          in the prime factorization of n.
 * https://oeis.org/A381204
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd{p_i, e_i : prime factorization n = ∏ p_i^e_i }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For n < 2 the sequence is undefined; return 1 for safety. */
    if (n < 2)
        return 1;

    /* Space for the largest possible number of prime factors of a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factor n.  factor_u64 writes the primes and exponents into
       primes[] and exps[], and sets cnt to the number of factors. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the gcd of all primes and all exponents.  Starting
       with zero works because gcd(0, x) = x. */
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        res = gcd(res, (seq_t)primes[i]);
        res = gcd(res, (seq_t)exps[i]);
    }

    return res;
}
