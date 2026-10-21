/*
 * A284118  Sum of nonprime squarefree divisors of n.
 * https://oeis.org/A284118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of non‑prime squarefree divisors of n. Compute all
 *            distinct prime factors of n, multiply (1+p) over them to get the
 *            sum of all squarefree divisors, subtract the sum of the primes
 *            themselves. The result includes 1 (which is non‑prime) and
 *            excludes all single‑prime divisors
 * Range:     u8 n <= 137, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t u = (uint64_t)n;
    /* The largest number of distinct primes in a 64‑bit integer is < 64. */
    uint64_t primes[64];
    uint64_t exps[64];       /* exponents are not needed for this computation */
    size_t   cnt = 0;

    /* Get the distinct prime factors of n. */
    factor_u64(u, primes, exps, &cnt);

    seq_t prod = 1;
    seq_t sum  = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        prod = prod * (p + 1);  /* multiply by (1+p) */
        sum  = sum + p;         /* accumulate Σ p   */
    }

    return prod - sum;          /* ∏(1+p) − Σ p  */
}
