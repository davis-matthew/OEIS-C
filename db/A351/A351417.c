/*
 * A351417  Number of divisors of n that are either prime or have at least one
 *          square divisor > 1.
 * https://oeis.org/A351417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are either prime or have at
 *            least one square divisor > 1. This equals (#prime divisors of n)
 *            + (#non‑squarefree divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n=0, harmless */
    uint64_t nn = (uint64_t)n;
    if (nn == 1) return 0;         /* a(1) = 0 */

    /* factor n: primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t total = 1;          /* total number of divisors */
    seq_t prime_count = 0;    /* number of distinct prime divisors */

    for (size_t i = 0; i < cnt; ++i) {
        total *= (seq_t)(exps[i] + 1);
        prime_count += 1;
    }

    /* number of squarefree divisors = 2^(prime_count) */
    seq_t squarefree = pow2( (idx_t)prime_count );

    seq_t non_squarefree = total - squarefree;
    return prime_count + non_squarefree;
}
