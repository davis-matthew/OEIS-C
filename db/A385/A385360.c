/*
 * A385360  Consecutive states of the linear congruential pseudo-random number
 *          generator (1861*s + 49297) mod 233280 when started at s=1.
 * https://oeis.org/A385360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = (1861 * a(n-1) + 49297) mod 233280
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;          /* allocated size (>= index+1) */
static idx_t max_comp = 0;           /* highest index computed */

/* Ensure the cache can hold terms up to index n */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n + 1)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < n + 1)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* cannot recover from memory exhaustion */
    cache = tmp;
    cache_cap = new_cap;

    /* Initialise base case if this is the first allocation */
    if (max_comp == 0) {
        cache[1] = (seq_t)1;
        max_comp = 1;
    }
}

/* Computes a(n) for the linear congruential generator */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n < 1 */

    ensure_cache(n);

    /* If the value is already computed, return it immediately */
    if (n <= max_comp)
        return cache[n];

    /* Compute successive terms up to n */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = (prev * (seq_t)1861 + (seq_t)49297) % (seq_t)233280;
        cache[i] = next;
    }
    max_comp = n;
    return cache[n];
}
