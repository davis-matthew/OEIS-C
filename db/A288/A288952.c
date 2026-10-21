/*
 * A288952  Number of relaxed compacted binary trees of right height at most
 *          one with empty sequences between branch nodes on level 0.
 * https://oeis.org/A288952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   seq_alloc, seq_free, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=0; a(n) = (n-1)*a(n-1)+(n-1)^2*a(n-2) */
#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_free.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cache_sz  = 0;   /* number of values currently stored  */
    static size_t cache_cap = 0;   /* allocated array capacity              */

    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = seq_alloc(2);            /* two elements: a(0) and a(1) */
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache_sz  = 2;
        cache_cap = 2;
    }

    /* value already computed? */
    if ((size_t)n < cache_sz) {
        return cache[(size_t)n];
    }

    /* enlarge cache if required */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = seq_realloc(cache, cache_cap, needed);
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache     = tmp;
        cache_cap = needed;
    }

    /* compute missing values */
    for (size_t i = cache_sz; i <= (size_t)n; ++i) {
        seq_t  prev  = cache[i - 1];
        seq_t  prev2 = cache[i - 2];
        seq_t  m     = (seq_t)(i - 1);     /* (i-1) as seq_t               */
        seq_t  m_sq  = m * m;
        cache[i] = m * prev + m_sq * prev2;
    }

    cache_sz = needed;   /* we now have a(0)…a(n) stored */

    return cache[(size_t)n];
}
