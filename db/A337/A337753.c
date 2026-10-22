/*
 * A337753  The number of n-digit numbers which are divisible by 3 and where
 *          all decimal digits are odd.
 * https://oeis.org/A337753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 6*a(n-1)-6*a(n-2)+5*a(n-3), a(1)=2, a(2)=8, a(3)=41
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* includes required for size_t, malloc, free */
#include <stddef.h>
#include <stdlib.h>

/* Helper to allocate or grow a static cache of seq_t values. */
static seq_t *cache = NULL;           /* array holding a(1) ... a(m) */
static size_t cache_cap = 0;          /* allocated capacity */
static size_t cache_len = 0;          /* number of values computed so far */

static void
ensure_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;

    /* grow capacity (double until big enough) */
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < needed)
        new_cap *= 2;

    /* allocate new array.  For arbitrary-precision builds, seq_alloc_new_array
     * takes care of object construction.  For builtin types, plain malloc works. */
#ifdef seq_alloc_new_array
    seq_t *new_arr = seq_alloc_new_array(new_cap);
#else
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    /* copy existing values */
    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];

    /* free old array if present */
    if (cache != NULL) {
#ifdef free_seq_array
        free_seq_array(cache);
#else
        free(cache);
#endif
    }

    cache     = new_arr;
    cache_cap = new_cap;
}

/* Compute the n-th term.  n is 1‑based with offset 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    size_t N = (size_t)n;          /* work in size_t */
    ensure_cache(N);

    /* extend cache up to N if needed */
    for (size_t i = cache_len; i < N; ++i) {
        if (i == 0)                /* a(1) */
            cache[i] = (seq_t)2;
        else if (i == 1)           /* a(2) */
            cache[i] = (seq_t)8;
        else if (i == 2)           /* a(3) */
            cache[i] = (seq_t)41;
        else {                     /* recurrence */
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            seq_t v3 = cache[i - 3];
            cache[i] = (seq_t)( ((seq_t)6 * v1 ) - ((seq_t)6 * v2 )
                               + ((seq_t)5 * v3) );
        }
    }
    cache_len = N;

    return cache[n - 1];
}
