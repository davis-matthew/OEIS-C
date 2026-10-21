/*
 * A251213  Number of (n+1)X(2+1) 0..1 arrays with no 2X2 subblock having zero
 *          or two 1s.
 * https://oeis.org/A251213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Recurrence for OEIS A251213:
 *   a(1)=21 , a(2)=63 , a(3)=191 , a(4)=589 , a(5)=1807
 *   a(n)=3*a(n-1)+4*a(n-2)+4*a(n-5)-12*a(n-3)   for n≥6
 *
 * The implementation caches computed values in a static array so that
 * calls may appear in any order, and reallocates only when necessary.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* 1‑based array of computed values   */
static idx_t  cache_size = 0;    /* largest n that has been computed   */

/* Allocate an array of seq_t objects, portable to C and C++. */
static seq_t *alloc_array(idx_t sz)
{
#if defined(__cplusplus)
    return new seq_t[sz];
#else
    return (seq_t *)malloc((size_t)sz * sizeof(seq_t));
#endif
}

/* Free an array of seq_t objects, portable to C and C++. */
static void free_array(seq_t *p)
{
#if defined(__cplusplus)
    delete [] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1; behaviour below is irrelevant */
        return 0;

    /* Return cached value if it is already available. */
    if (n <= cache_size)
        return cache[n];

    /* Allocate a new array large enough for indices 0..n. */
    idx_t new_len = n + 1;                    /* index 0 unused */
    seq_t *new_cache = alloc_array(new_len);
    if (!new_cache)                          /* allocation failure */
        return 0;

    /* Copy previously computed values to the new array. */
    for (idx_t i = 1; i <= cache_size; ++i)
        new_cache[i] = cache[i];

    /* Base values for the first five terms. */
    if (n >= 1) new_cache[1] = (seq_t)21;
    if (n >= 2) new_cache[2] = (seq_t)63;
    if (n >= 3) new_cache[3] = (seq_t)191;
    if (n >= 4) new_cache[4] = (seq_t)589;
    if (n >= 5) new_cache[5] = (seq_t)1807;

    /* Compute further terms using the recurrence. */
    if (n >= 6) {
        idx_t start = cache_size + 1;
        if (start < 6) start = 6;            /* start at the first term that uses the recurrence */
        for (idx_t i = start; i <= n; ++i) {
            /* Use a form that avoids intermediate negative operands on unsigned types. */
            new_cache[i] = 3 * new_cache[i - 1]
                         + 4 * new_cache[i - 2]
                         + 4 * new_cache[i - 5]
                         - 12 * new_cache[i - 3];
        }
    }

    /* Replace the old cache with the new one. */
    if (cache)
        free_array(cache);
    cache      = new_cache;
    cache_size = n;

    return cache[n];
}
