/*
 * A377074  E.g.f. satisfies A(x) = exp( 3 * (exp(x*A(x)^(1/3)) - 1) ).
 * https://oeis.org/A377074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * Σ_{k=0..n} (n+3)^(k-1) * Stirling2(n,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

/* Cached values of the sequence */
static seq_t *a_cache = NULL;
static idx_t  cache_size = 0;   /* number of cached entries stored */
static idx_t  cache_cap   = 0;   /* current allocation capacity */

/* Cached Stirling numbers of the second kind (row of S(n,k)) */
static seq_t *stirling   = NULL;
static idx_t  stirling_sz = 0;  /* array capacity */

/* Ensure an array has capacity for at least `needed` elements.
   The pointer is updated if the allocation grows. */
static void ensure_capacity(idx_t *cap, idx_t needed, seq_t **arr)
{
    if (*cap < needed) {
        idx_t newcap = (*cap == 0) ? 1 : *cap;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!tmp) exit(1);            /* allocation failed */
        *arr = tmp;
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* First call: initialise caches */
    if (cache_size == 0) {
        ensure_capacity(&cache_cap, 1, &a_cache);
        a_cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_size = 1;

        ensure_capacity(&stirling_sz, 1, &stirling);
        stirling[0] = (seq_t)1;          /* S(0,0) = 1 */
    }

    /* Grow caches up to index n */
    while (cache_size <= n) {
        idx_t cur = cache_size;          /* index to compute */

        /* Ensure Stirling array can hold row cur+1 */
        ensure_capacity(&stirling_sz, cur + 1, &stirling);

        if (cur > 0) {
            /* Update Stirling numbers in place: from high k to low k.
               After the loop, stirling[cur] holds S(cur,cur) = 1 and
               stirling[0] will be set to 0. */
            for (idx_t k = cur; k > 0; k--) {
                seq_t term  = mul_small(stirling[k], (unsigned int)k);
                term += stirling[k - 1];
                stirling[k] = term;
            }
            stirling[0] = (seq_t)0;   /* S(cur,0) = 0 for cur>0 */
        }

        /* Compute a(cur) = 3 * Σ_{k=1..cur} (cur+3)^(k-1) * S(cur,k) */
        seq_t sum   = (seq_t)0;
        seq_t power = (seq_t)1;                /* (cur+3)^(k-1), start k=1 */
        unsigned int base = (unsigned int)(cur + 3);

        for (idx_t k = 1; k <= cur; k++) {
            sum += power * stirling[k];
            power = mul_small(power, base);
        }
        seq_t a_cur = mul_small(sum, 3);

        /* Append to cache */
        ensure_capacity(&cache_cap, cache_size + 1, &a_cache);
        a_cache[cache_size] = a_cur;
        cache_size++;
    }

    return a_cache[n];
}
