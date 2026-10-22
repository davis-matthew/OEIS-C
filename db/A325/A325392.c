/*
 * A325392  Number of permutations of the multiset of prime factors of n whose
 *          first part is not 2.
 * https://oeis.org/A325392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A008480(n) - [n even] * A008480(n/2)
 * Range:     u8 n <= 95, u16 n <= 9899, u32 n <= 9899, u64 n <= 9899,
 *            u128 n <= 9899, big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A008480(n) = Ω(n)! / ∏(e_i)! and Ω(n) = Σ e_i for n = ∏ p_i^e_i */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/factorial.h"

/* Compute the number of distinct permutations of the multiset of prime factors of n. */
static seq_t perm_prime_factors(uint64_t n)
{
    /* The largest number of distinct prime factors of a 64‑bit integer is < 16,
       but we allow a safe buffer of 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    /* total exponent sum Ω(n) */
    idx_t omega = 0;
    for (size_t i = 0; i < cnt; ++i)
        omega += (idx_t)exps[i];

    seq_t numerator = factorial(omega);
    seq_t denominator = 1;

    for (size_t i = 0; i < cnt; ++i)
        denominator *= factorial((idx_t)exps[i]);

    return numerator / denominator;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t total = perm_prime_factors((uint64_t)n);
    if ((n & 1U) == 0) {                 /* n even */
        seq_t sub = perm_prime_factors((uint64_t)(n >> 1));
        return total - sub;
    }
    return total;                         /* n odd */
}
