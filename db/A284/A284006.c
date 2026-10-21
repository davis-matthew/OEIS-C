/*
 * A284006  a(n) = a(a(n-A001511(n))) + a(n-a(n-A001511(n))) with a(1) = a(2)
 *          = 1, where A001511(n) = 1 + exponent of the highest power of 2
 *          dividing n.
 * https://oeis.org/A284006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-A001511(n))) + a(n-a(n-A001511(n))) , a(1)=a(2)=1
 * Range:     u8 n <= 401, u16 n <= 9632, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A001511(n) = 1 + exponent of the highest power of 2 dividing n */
/* The function memoises the sequence so that calls may appear in any order */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* holds a(i) for 1 ≤ i ≤ cache_cap */
static idx_t cache_cap = 0;      /* allocated capacity (≥ the largest index stored) */
static idx_t computed_up_to = 0; /* largest index that has already been computed */

/* Ensure cache can hold entries up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return; /* enough space */

    idx_t newcap = cache_cap ? cache_cap * 2 : 4;
    while (newcap < n + 1)
        newcap <<= 1; /* double until large enough */

    seq_t *newcache = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
    if (!newcache)
        abort(); /* allocation failure */

    /* new entries start with zero (fits if seq_t is numeric or trivially initialised) */
    for (idx_t i = cache_cap + 1; i <= newcap; ++i)
        newcache[i] = 0;

    cache = newcache;
    cache_cap = newcap;
}

/* Return the index of a(n) values; compute iteratively as needed */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n < 1, but harmless */

    if (n < 3)
        return 1; /* a(1) = a(2) = 1 */

    ensure_cache(n);

    /* initialise base values once */
    if (cache[1] == 0) {
        cache[1] = 1;
        cache[2] = 1;
        computed_up_to = 2;
    }

    /* compute all values up to n iteratively */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* exponent of highest power of 2 dividing i */
        int tz = __builtin_ctzll((uint64_t)i); /* v2(i) */
        idx_t t = i - (1 + (idx_t)tz);        /* i - A001511(i) */

        /* a(t) value is already computed, and is small enough to use as an index */
        idx_t a_t = (idx_t)cache[t];
        seq_t val1 = cache[a_t];              /* a(a(t)) */

        /* n - a(t) is also ≤ i-1, thus already computed */
        idx_t idx2 = i - a_t;
        seq_t val2 = cache[idx2];              /* a(i - a(t)) */

        cache[i] = val1 + val2;
    }

    computed_up_to = n;
    return cache[n];
}
