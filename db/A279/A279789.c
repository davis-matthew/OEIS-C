/*
 * A279789  Number of ways to choose a constant partition of each part of a
 *          constant partition of n.
 * https://oeis.org/A279789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 0, otherwise a(n) = Σ_{d|n} τ(n/d)^d where τ(k)
 *            is the number of divisors of k
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes divisor counts τ(k) up to the largest n
   requested, then iterates over the divisors of n to accumulate the sum.
   It uses the utility function pow_seq(seq_t base, int exp) available
   from "../utils/pow_seq.h".  For small n this is well within the
   required time limit. */

#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "../utils/pow_seq.h"

/* static cache of divisor counts */
static seq_t *tau_cache = NULL;   /* τ(k) for 0 <= k <= cached_max */
static idx_t cached_max  = 0;

/* recompute τ(k) for all k <= n using the divisor summation sieve */
static void recompute_tau(idx_t n)
{
    /* free previous cache */
    if (tau_cache)
        free(tau_cache);

    tau_cache = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tau_cache)
        abort(); /* out of memory */

    /* initialise and compute */
    for (idx_t i = 0; i <= n; ++i)
        tau_cache[i] = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        for (idx_t j = i; j <= n; j += i)
            tau_cache[j] += 1;
    }

    cached_max = n;
}

/* ensure τ(k) is computed up to n */
static void ensure_tau(idx_t n)
{
    if (n <= cached_max)
        return;
    recompute_tau(n);
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    ensure_tau(n);

    seq_t result = (seq_t)0;

    /* iterate over divisors */
    for (idx_t i = 1; (i * i) <= n; ++i)
    {
        if (n % i != 0)
            continue;

        idx_t d1 = i;            /* first divisor */
        idx_t d2 = n / i;        /* complementary divisor */

        /* term for divisor d1: τ(n/d1)^d1 = τ(d2)^d1 */
        seq_t base1 = tau_cache[d2];
        seq_t term1 = pow_seq(base1, (int)d1);
        result += term1;

        if (d1 != d2)
        {
            /* term for divisor d2: τ(n/d2)^d2 = τ(d1)^d2 */
            seq_t base2 = tau_cache[d1];
            seq_t term2 = pow_seq(base2, (int)d2);
            result += term2;
        }
    }

    return result;
}
