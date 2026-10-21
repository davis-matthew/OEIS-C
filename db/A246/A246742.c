/*
 * A246742  Number of length 5+4 0..n arrays with no pair in any consecutive
 *          five terms totalling exactly n.
 * https://oeis.org/A246742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   dp_base_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A246742
   Formula: a(n) = 3*a(n-1) +4*a(n-2) -20*a(n-3) +56*a(n-5) -28*a(n-6) -84*a(n-7) +70*a(n-8) +70*a(n-9)
               -84*a(n-10) -28*a(n-11) +56*a(n-12) -20*a(n-14) +4*a(n-15) +3*a(n-16) -a(n-17)
   Initial values for 1 ≤ n ≤ 17 are obtained by a direct DP from the definition. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/dp_base_value.h"

/* -------------------------------------------------------------------- */
/*  Cache structure                                                    */
/* -------------------------------------------------------------------- */
static seq_t *cache = NULL;      /* 1‑based cache of computed terms */
static idx_t cache_len = 0;      /* highest n computed so far  */
static size_t cache_cap = 0;     /* allocated capacity (indices 0..cache_cap) */

/* Ensure the cache can store indexes up to n (1‑based).  */
static void ensure_cache_size(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < (size_t)n)
        new_cap *= 2;

    seq_t *ptr = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!ptr)
        abort();
    /* initialise new slots to 0 */
    for (size_t i = cache_cap + 1; i <= new_cap; ++i)
        ptr[i] = 0;
    cache = ptr;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------------- */
/*  DP for n ≤ 17  –  counts arrays 0..n avoiding a pair summing to n    */

/* -------------------------------------------------------------------- */
/*  Fill cache for n = 1..17                                           */
/* -------------------------------------------------------------------- */
static void fill_base_cache(void)
{
    ensure_cache_size(17);
    for (idx_t i = 1; i <= 17; ++i)
        cache[i] = dp_base_value(i);
    cache_len = 17;
    cache[0] = 0;      /* convenience for recurrence */
}

/* -------------------------------------------------------------------- */
/*  Public function                                                    */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    if (!cache)
        fill_base_cache();       /* initialise base values */

    if (n <= cache_len)
        return cache[n];

    ensure_cache_size(n);

    /* compute terms from cache_len+1 up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos, neg, res;

        /* positive contributions */
        pos = 0;
        pos += cache[i - 1] * (seq_t)3;
        pos += cache[i - 2] * (seq_t)4;
        pos += cache[i - 5] * (seq_t)56;
        pos += cache[i - 8] * (seq_t)70;
        pos += cache[i - 9] * (seq_t)70;
        pos += cache[i -12] * (seq_t)56;
        pos += cache[i -15] * (seq_t)4;
        pos += cache[i -16] * (seq_t)3;

        /* negative contributions */
        neg = 0;
        neg += cache[i - 3] * (seq_t)20;
        neg += cache[i - 6] * (seq_t)28;
        neg += cache[i - 7] * (seq_t)84;
        neg += cache[i -10] * (seq_t)84;
        neg += cache[i -11] * (seq_t)28;
        neg += cache[i -14] * (seq_t)20;
        neg += cache[i -17] * (seq_t)1;

        /* final value (positive ≥ negative by construction) */
        res = pos - neg;
        cache[i] = res;
    }

    cache_len = n;
    return cache[n];
}
