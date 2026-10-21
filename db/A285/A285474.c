/*
 * A285474  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 3", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A285474:
   a(0)=1 , a(1)=11 , a(2)=100 , a(3)=1111
   for n>3:  a(n) = 101*a(n-2) - 100*a(n-4)          (OEIS conjecture)   */

#include <stdlib.h>
#include <string.h>  /* for memcpy */

static seq_t *cache = NULL;   /* dynamically expanded cache of terms */
static size_t cache_size = 0; /* number of cached entries (>=1 for 0..cache_size-1) */

seq_t a(idx_t n)
{
    /* Extend the cache if necessary */
    size_t old_size = cache_size;
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;          /* need indices 0 .. n */
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;                 /* allocation failed – return 0 */
        if (cache)
        {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing entries using the recurrence */
    for (size_t i = old_size; i <= (size_t)n; ++i)
    {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)      cache[i] = (seq_t)11;
        else if (i == 2)      cache[i] = (seq_t)100;
        else if (i == 3)      cache[i] = (seq_t)1111;
        else
        {
            /* Recurrence: a(n) = 101*a(n-2) - 100*a(n-4) */
            cache[i] = (seq_t)101 * cache[i - 2] - (seq_t)100 * cache[i - 4];
        }
    }
    return cache[n];
}
