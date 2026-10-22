/*
 * A391904  a(n) = Sum_{k=0..floor(n/3)} binomial(3*k,3*(n-3*k)).
 * https://oeis.org/A391904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 40, u64 n <= 78, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..floor(n/3)} binomial(3*k, 3*(n-3*k))
 *
 * Recurrence (for n ≥ 12):
 *   a(n) = 3*a(n-3) + 3*a(n-4) - 3*a(n-6) + 21*a(n-7)
 *          - 3*a(n-8) + a(n-9) + 3*a(n-10) + 3*a(n-11) + a(n-12)
 *
 * Initial values:
 *   a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=0, a(6)=1,
 *   a(7)=20, a(8)=1, a(9)=1, a(10)=84, a(11)=84
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache   = NULL;      /* pointer to array of a(n) values      */
static size_t cache_cap = 0;       /* allocated capacity (in terms of n)   */
static size_t cache_len = 0;       /* number of values actually stored    */

/* Return the first 12 terms of the sequence */
static inline seq_t base_value(idx_t k)
{
    switch (k)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)0;
        case 3: return (seq_t)1;
        case 4: return (seq_t)1;
        case 5: return (seq_t)0;
        case 6: return (seq_t)1;
        case 7: return (seq_t)20;
        case 8: return (seq_t)1;
        case 9: return (seq_t)1;
        case 10: return (seq_t)84;
        case 11: return (seq_t)84;
    }
    return (seq_t)0;                /* should never be reached          */
}

/* Ensure that values up to n are computed and cached */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)                 /* already computed                */
        return;

    /* --- grow the cache if necessary ----------------------------------- */
    size_t required = (size_t)n + 1;   /* we need indices 0..n inclusive  */
    if (required > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < required)
            new_cap <<= 1;             /* double until big enough           */

        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)                /* allocation failure is fatal       */
            abort();

        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        free(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* --- compute missing values ---------------------------------------- */
    for (size_t i = cache_len; i < required; ++i)
    {
        if (i < 12)
            cache[i] = base_value((idx_t)i);
        else
            cache[i] = 3*cache[i-3] + 3*cache[i-4] - 3*cache[i-6] +
                       21*cache[i-7] - 3*cache[i-8] + cache[i-9] +
                       3*cache[i-10] + 3*cache[i-11] + cache[i-12];
    }
    cache_len = required;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
