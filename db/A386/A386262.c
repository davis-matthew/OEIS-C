/*
 * A386262  a(n) = A051903(A051903(n)) for n >= 2, a(1) = 0.
 * https://oeis.org/A386262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A051903(A051903(n)) for n >= 2, a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Return the largest prime‑exponent occurring in the factorisation of n.
   For n = 0 or 1 the result is 0.  */
static inline idx_t max_exp_idx(idx_t n)
{
    if (n <= 1)
        return 0;

    size_t cnt;
    uint64_t primes[64];
    uint64_t exps[64];

    /* factor_u64 returns the distinct prime factors of n in @primes
       together with their exponents in @exps, and the number of
       factors in @cnt.  */
    factor_u64(n, primes, exps, &cnt);

    idx_t max = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if ((idx_t)exps[i] > max)
            max = (idx_t)exps[i];
    }
    return max;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    idx_t e1 = max_exp_idx(n);   /* A051903(n) */
    idx_t e2 = max_exp_idx(e1);  /* A051903(A051903(n)) */
    return (seq_t)e2;
}
