/*
 * A302017  Expansion of 1/(1 - x*Product_{k>=1} (1 + x^(2*k-1))).
 * https://oeis.org/A302017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302017:  Expansion of 1/(1 - x*∏_{k≥1} (1 + x^(2*k-1)))
 *  Recurrence: a(0) = 1;
 *  For n ≥ 1:
 *    a(n) = Σ_{k=1..n} A000700(k−1) * a(n−k),
 *    where A000700(m) is the number of partitions of m into
 *    distinct odd parts.
 *  This implementation memoizes a(n) and recomputes the
 *  A000700 coefficients from scratch whenever new indices are required.
 */

#include <stdbool.h>
#include "../utils/ensure_cache.h"

/* memoization tables */
static seq_t *a_cache = NULL;   /* a(n) values               */
static size_t a_cap  = 0;       /* capacity of a_cache        */
static idx_t  a_max  = 0;       /* largest n already computed */

/* A000700 cache: q_cache[m] = A000700(m) */
static seq_t *q_cache = NULL;
static size_t q_cap  = 0;
static idx_t  q_max  = 0;

/* rebuild A000700 values for indices 0..target_q */
static void rebuild_q(idx_t target_q)
{
    /* allocate enough space */
    ensure_cache(target_q + 1, &q_cache, &q_cap);

    /* initialise all entries to 0 */
    for (idx_t i = 0; i <= target_q; ++i)
        q_cache[i] = (seq_t)0;

    /* base case: partition of 0 */
    q_cache[0] = (seq_t)1;

    /* process all odd parts up to target_q */
    for (idx_t part = 1; part <= target_q; part += 2)
    {
        for (idx_t t = target_q; t >= part; --t)
        {
            q_cache[t] += q_cache[t - part];
        }
    }
    q_max = target_q;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* initialise a_cache */
    if (a_cache == NULL)
    {
        ensure_cache(1, &a_cache, &a_cap);
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* initialise q_cache */
    if (q_cache == NULL)
    {
        ensure_cache(1, &q_cache, &q_cap);
        q_cache[0] = (seq_t)1;
        q_max = 0;
    }

    /* trivial case */
    if (n == 0)
        return a_cache[0];

    /* ensure q_cache covers indices up to n-1 */
    if (n - 1 > q_max)
    {
        rebuild_q(n - 1);
    }

    /* ensure a_cache has space for index n */
    if ((size_t)n + 1 > a_cap)
        ensure_cache((size_t)n + 1, &a_cache, &a_cap);

    /* compute a(n) incrementally */
    for (idx_t i = a_max + 1; i <= n; ++i)
    {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k)
        {
            sum += q_cache[k - 1] * a_cache[i - k];
        }
        a_cache[i] = sum;
    }
    a_max = n;

    return a_cache[n];
}
