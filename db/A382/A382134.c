/*
 * A382134  Number of completely asymmetric matchings (not containing centered
 *          or coupled arcs) of [2n].
 * https://oeis.org/A382134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * A001205(n) * Recurrence: a(0)=1, a(1)=0, a(2)=0 *
 *            a(n) = 2*(n-1)*a(n-1) + 4*(n-1)*(n-2)*a(n-3) for n>=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"    /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t*,size_t,size_t) */

static seq_t *cache = NULL;   /* array of computed a(n) values */
static size_t cache_cap = 0;  /* current capacity of cache array */
static size_t cache_len = 0;  /* number of values actually filled (last index+1) */

/* Ensure cache is initialized and large enough to hold values up to n. */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation */
        cache_cap = 128;                       /* start with some size */
        cache = seq_alloc(cache_cap);
        /* base cases */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache_len = 3;
    }

    /* grow cache if necessary */
    idx_t required = n + 1;
    if ((idx_t)cache_cap < required) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)required) new_cap = (size_t)required;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1 || i == 2) {
            cache[i] = (seq_t)0;
        } else {
            seq_t prev   = cache[i - 1];
            seq_t prev3  = cache[i - 3];
            seq_t term1  = (seq_t)2 * (seq_t)(i - 1) * prev;
            seq_t term2  = (seq_t)4 * (seq_t)(i - 1) * (seq_t)(i - 2) * prev3;
            cache[i] = term1 + term2;
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    return cache[n];
}
