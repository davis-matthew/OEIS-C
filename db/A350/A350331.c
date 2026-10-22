/*
 * A350331  Number of divisors of n that are prime or the product of 2 (not
 *          necessarily distinct) primes.
 * https://oeis.org/A350331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k + C(k,2) + k2 where k = number of distinct prime
 *            factors of n k2 = number of distinct primes p with exponent >= 2
 *            in n This counts the divisors of n that have exactly 1 or 2
 *            prime factors (counted with multiplicity), i.e. the primes and
 *            semiprimes (p² or p·q)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
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
    if (n < 1) return 0;   /* sequence defined for n ≥ 1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t k  = (seq_t)cnt; /* distinct prime count */

    seq_t k2 = 0;          /* primes with exponent ≥ 2 */
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] >= 2) ++k2;

    seq_t comb = 0;        /* C(k,2) */
    if (k >= 2)
        comb = (k * (k - 1)) / 2;

    return k + comb + k2;
}
