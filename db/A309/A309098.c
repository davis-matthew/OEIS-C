/*
 * A309098  Number of partitions of n avoiding the partition (4,3).
 * https://oeis.org/A309098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A309098: Number of partitions of n avoiding the partition (4,3)
 *  Recurrence derived from the generating function:
 *      A(x) = (1 + Σ_{i≥3} x^i/(1-x^i)) / (1 - x - x^2 + x^3)
 *  which yields for n ≥ 3:
 *      a(n) = a(n-1) + a(n-2) - a(n-3) + d(n)
 *  where d(n) = τ(n) – 1 – [2|n]  (τ(n) = number of divisors of n).
 *  Initial values: a(0)=1, a(1)=1, a(2)=2.
 *  The implementation memoises previously computed terms and pre‑computes
 *  a divisor–count sieve up to the requested n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* cache of divisor counts (τ) -------------------------- */
static uint32_t *tau_cache   = NULL;   /* τ(k) for 0 ≤ k ≤ tau_max   */
static idx_t    tau_max      = 0;      /* largest k for which τ is known */

/* ensure that tau_cache contains divisor counts up to n */
static void ensure_tau(idx_t n)
{
    if (n <= tau_max)
        return;

    free(tau_cache);                     /* discard old table */
    size_t sz = (size_t)(n + 1);
    tau_cache = (uint32_t*)calloc(sz, sizeof(uint32_t));
    if (!tau_cache)
        abort();                        /* memory allocation failed */

    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++tau_cache[j];

    tau_max = n;
}

/* cache of sequence values -------------------------------- */
static seq_t *cache          = NULL;    /* memoised a(k) for 0 ≤ k ≤ cache_max */
static size_t cache_cap      = 0;       /* allocated capacity in terms of a() */
static idx_t  cache_max      = 0;       /* largest index whose value is known */

/* ensure that cache has space for indices up to n */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= (size_t)(n + 1))
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache)
        abort();

    /* copy existing values */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* free old array and update pointers */
    free_seq_array(cache);
    cache       = new_cache;
    cache_cap   = new_cap;
}

/* public function --------------------------------------------------- */
seq_t a(idx_t n)
{
    /* base cases handled directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    /* initialise cache if necessary */
    if (!cache) {
        cache_cap = 3;                    /* capacity for indices 0..2 */
        cache     = alloc_seq_array(cache_cap);
        if (!cache)
            abort();

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_max = 2;
    }

    /* grow cache if needed */
    ensure_cache(n);

    /* pre‑compute divisor counts up to n */
    ensure_tau(n);

    /* extend memoised sequence up to n */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val = cache[k - 1] + cache[k - 2];
        if (k >= 3u)
            val -= cache[k - 3];

        if (k >= 3u) {
            uint32_t tau_val = tau_cache[k];
            seq_t d = (seq_t)( tau_val
                               - 1u
                               - ( (k & 1u) == 0u ? 1u : 0u ) );
            val += d;
        }

        cache[k] = val;
    }

    cache_max = n;
    return cache[n];
}
