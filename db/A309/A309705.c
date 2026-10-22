/*
 * A309705  a(n) = lcm(a(n-1), n) - gcd(a(n-1), n) where a(1) = 1.
 * https://oeis.org/A309705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 16, u64 n <= 25, u128 n <= 40,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = lcm(a(n-1), n) - gcd(a(n-1), n) (n > 1) */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdint.h>   /* uint64_t used for idx_t conversion if needed */
#include <stdbool.h>  /* not used directly but required by the style guide */
#include "../utils/gcd.h"

/* Return the n-th term of sequence A309705.  The function caches
   previously computed values so that calls in arbitrary order are
   efficient.  All arithmetic is done with seq_t, which may be an
   arbitrary‑precision integer type or an unsigned integer of 8, 16,
   32, 64 or 128 bits. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;
    static idx_t cache_len = 0;   /* number of stored elements */
    static idx_t cache_cap = 0;   /* allocated capacity          */

    /* Handle the offset: a(1) is the first term.  Behaviour for n < 1
       is irrelevant; return 0 for safety. */
    if (n < 1ULL) return (seq_t)0;

    /* Initialise the cache with a(1) if it is empty. */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;    /* a(1) */
        cache_len = 1;
    }

    /* If the requested value is already computed, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache if necessary.  The new capacity is at least n. */
    if (n > cache_cap) {
        cache_cap = n;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute the missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];                      /* a(i-1)  */
        seq_t g    = gcd(prev, (seq_t)i);               /* gcd(a(i-1), i) */
        seq_t lcm  = (prev / g) * (seq_t)i;             /* lcm = (a/g)*i */
        cache[i - 1] = lcm - g;                         /* a(i) = lcm - g */
    }

    cache_len = n;
    return cache[n - 1];
}
