/*
 * A375308  a(n) = Sum_{k=0..floor(2*n/3)} binomial(4*n-4*k,2*k).
 * https://oeis.org/A375308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A375308: a(n) = Sum_{k=0..⌊2n/3⌋} binomial(4n-4k, 2k)
   Equivalent recurrence:
     a(0)=1, a(1)=1, a(2)=7, a(3)=30, a(4)=137, a(5)=644
     a(n)=2*a(n-1)+11*a(n-2)+6*a(n-3)-6*a(n-4)+4*a(n-5)-a(n-6)   (n≥6) */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static idx_t computed_upto = 0;

/* Helper to grow cache to at least 'need' elements */
static void ensure_cache_size(idx_t need)
{
    if (cache_len >= (size_t)need)
        return;
    size_t new_len = cache_len ? cache_len : 16;
    while (new_len < (size_t)need)
        new_len *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_len = 32;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        /* First six terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)7;
        cache[3] = (seq_t)30;
        cache[4] = (seq_t)137;
        cache[5] = (seq_t)644;
        computed_upto = 5;   /* Indices 0..5 have been computed */
    }

    /* Ensure cache can hold requested index */
    if ((size_t)n >= cache_len)
        ensure_cache_size((size_t)n + 1);

    /* Compute missing values up to n */
    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        /* For i < 6 the values are already initialised */
        if (i < 6) {
            cache[i] = cache[i];
            continue;
        }
        /* Recurrence:
           a(n)=2*a(n-1)+11*a(n-2)+6*a(n-3)-6*a(n-4)+4*a(n-5)-a(n-6) */
        seq_t sum_plus  = cache[i-1] * (seq_t)2
                        + cache[i-2] * (seq_t)11
                        + cache[i-3] * (seq_t)6
                        + cache[i-5] * (seq_t)4;
        seq_t sum_minus = cache[i-4] * (seq_t)6
                        + cache[i-6];
        cache[i] = sum_plus - sum_minus;
    }
    computed_upto = n;
    return cache[n];
}
