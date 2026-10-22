/*
 * A333145  Number of unimodal negated permutations of the multiset of prime
 *          indices of n.
 * https://oeis.org/A333145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n that are not divisible by the
 *            least prime factor of n. Equivalently, a(n) = d(n) / (e1 + 1),
 *            where d(n) is the divisor count of n and e1 is the exponent of
 *            the smallest prime factor of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;          /* a(1) = 1 by definition */

    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Find smallest prime factor and its exponent */
    uint64_t min_p = primes[0];
    uint64_t min_exp = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (primes[i] < min_p) {
            min_p = primes[i];
            min_exp = exps[i];
        }
    }

    /* Compute total number of divisors d(n) = product (exp_i + 1) */
    seq_t div_count = 1;
    for (size_t i = 0; i < cnt; ++i) {
        div_count *= (seq_t)(exps[i] + 1);
    }

    /* a(n) = d(n) / (e1 + 1) */
    seq_t result = div_count / ((seq_t)(min_exp + 1));
    return result;
}
