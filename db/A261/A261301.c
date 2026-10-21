/*
 * A261301  a(n+1) = abs(a(n) - gcd(a(n), n)), a(1) = 1.
 * https://oeis.org/A261301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 312, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = |a(n) - gcd(a(n), n)| for n ≥ 1 */
/* The function memoises previously computed values. */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;   /* array holding a(1) .. a(cache_size) */
static idx_t cache_size = 0;  /* number of terms currently cached */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* Return the n-th term (n ≥ 1) of OEIS A261301. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* undefined for indices < 1 */

    /* First‑time initialisation */
    if (cache == NULL) {
        cache_cap = 4;                /* start with room for 4 terms */
        cache = seq_realloc(NULL, 0, cache_cap);
        if (!cache)
            abort();
        cache[0] = 1;                 /* a(1) = 1 */
        cache_size = 1;
    }

    /* Grow the cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to index n */
    while (cache_size < n) {
        idx_t idx = cache_size + 1;            /* next 1‑based index to compute */
        seq_t prev = cache[cache_size - 1];     /* a(idx-1) */
        seq_t g = gcd(prev, (seq_t)(idx - 1));  /* gcd(a(idx-1), idx-1) */
        seq_t next = (prev >= g) ? (prev - g) : (g - prev); /* |prev - g| */
        cache[cache_size] = next;              /* store a(idx) */
        ++cache_size;
    }

    return cache[n - 1]; /* cache[0] holds a(1) */
}
