/*
 * A248070  Number of length 2+5 0..n arrays with some disjoint triples in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 43, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Empirical recurrence for A248070:
   a(n) = 3*a(n-2) + 2*a(n-3) - 2*a(n-4) - 6*a(n-5) - 3*a(n-6)
          + 4*a(n-7) + 6*a(n-8) + 4*a(n-9) - 3*a(n-10) - 6*a(n-11)
          - 2*a(n-12) + 2*a(n-13) + 3*a(n-14) - a(n-16)
   (for n ≥ 17), with initial values:
   a(1)=32, a(2)=513, a(3)=3364, a(4)=15125, a(5)=48316,
   a(6)=131677, a(7)=299968, a(8)=625269, a(9)=1174080,
   a(10)=2085341, a(11)=3462212, a(12)=5539433, a(13)=8458164,
   a(14)=12578465, a(15)=18080936, a(16)=25484457
*/

#include <stddef.h>
#include <stdlib.h>  /* for malloc/realloc */

static const seq_t init[16] = {
    (seq_t)32,          /* a(1) */
    (seq_t)513,         /* a(2) */
    (seq_t)3364,        /* a(3) */
    (seq_t)15125,       /* a(4) */
    (seq_t)48316,       /* a(5) */
    (seq_t)131677,      /* a(6) */
    (seq_t)299968,      /* a(7) */
    (seq_t)625269,      /* a(8) */
    (seq_t)1174080,     /* a(9) */
    (seq_t)2085341,     /* a(10) */
    (seq_t)3462212,     /* a(11) */
    (seq_t)5539433,     /* a(12) */
    (seq_t)8458164,     /* a(13) */
    (seq_t)12578465,    /* a(14) */
    (seq_t)18080936,    /* a(15) */
    (seq_t)25484457     /* a(16) */
};

static seq_t *cache = NULL;      /* cached sequence values (0-indexed) */
static size_t cache_len = 0;     /* number of values already stored */
static size_t cache_cap = 0;     /* capacity of the cache array */

/* Helper: guarantee cache has capacity for at least n elements (1‑based n). */
static void
ensure_cache_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap < (size_t)n) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n ≥ 1.  The function caches values for O(n) total cost. */
seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 */

    /* Allocate/make sure cache is big enough. */
    ensure_cache_capacity(n);

    /* Pre‑populate the cache with the known initial values if needed. */
    if (cache_len < 16) {
        size_t i, limit = 16;
        if (limit > (size_t)n) limit = (size_t)n;
        for (i = cache_len; i < limit; ++i)
            cache[i] = init[i];
    }

    /* Compute missing terms up to n. */
    for (size_t j = cache_len; j < (size_t)n; ++j) {
        if (j < 16) {  /* should already be set from init */
            /* nothing to do */
        } else {
            /* Compute using the recurrence, avoiding negative intermediate
               values by separating positives and negatives. */
            seq_t pos = cache[j-2]*3 + cache[j-3]*2
                + cache[j-7]*4 + cache[j-8]*6 + cache[j-9]*4
                + cache[j-13]*2 + cache[j-14]*3;
            seq_t neg = cache[j-4]*2 + cache[j-5]*6 + cache[j-6]*3
                + cache[j-10]*3 + cache[j-11]*6 + cache[j-12]*2
                + cache[j-16];
            cache[j] = pos - neg;   /* pos ≥ neg by the recurrence */
        }
    }

    cache_len = (size_t)n;   /* all values up to n are now computed */
    return cache[(size_t)n - 1];
}
