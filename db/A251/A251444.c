/*
 * A251444  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the minimum of its diagonal elements less than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A251444: Number of (n+1)×2 arrays over {0,1,2} with
   the condition that for every 2×2 subblock the minimum of its
   diagonal elements is not less than the absolute difference of
   its antidiagonal elements.
   Recurrence (offset 1):
        a(n) = 5*a(n-1) + 4*a(n-2) - 11*a(n-3) - 6*a(n-4) + 5*a(n-5) + 4*a(n-6),
        a(1)=45, a(2)=230, a(3)=1223, a(4)=6505,
        a(5)=34674, a(6)=184818.
*/

#include <stdlib.h>
#include <stddef.h>

/* seq_t and idx_t are defined by the build environment. */

static seq_t *seq_cache = NULL;   /* cache[0] unused, cache[1]..cache[n] = a(n) */
static idx_t  seq_cache_size = 0; /* last computed index */

/* Retrieve a(n) for n>=1 using memoised recurrence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for 0, but return 0 safely */

    /* Initialise the cache if it is the first call. */
    if (seq_cache == NULL) {
        /* Allocate space for the first 6 terms (indices 0..6). */
        seq_cache = (seq_t *)malloc((size_t)(6 + 1) * sizeof(seq_t));
        if (!seq_cache) return 0;       /* allocation failed, return 0 */
        seq_cache[0] = 0;               /* dummy, never used in recurrence */
        seq_cache[1] = 45;
        seq_cache[2] = 230;
        seq_cache[3] = 1223;
        seq_cache[4] = 6505;
        seq_cache[5] = 34674;
        seq_cache[6] = 184818;
        seq_cache_size = 6;
    }

    /* If the requested n is already cached, return it immediately. */
    if (n <= seq_cache_size) {
        return seq_cache[n];
    }

    /* Ensure the cache has room for indices up to n. */
    if (n + 1 > seq_cache_size + 1) {
        /* Reallocate to size n+1 (indices 0..n). */
        seq_t *tmp = (seq_t *)realloc(seq_cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;             /* reallocation failed */
        seq_cache = tmp;
        /* No need to initialise new entries because they will be filled below. */
    }

    /* Compute missing terms up to n, appending to the cache. */
    for (idx_t k = seq_cache_size + 1; k <= n; ++k) {
        /* Use the recurrence relation. */
        seq_t pos = 5 * seq_cache[k - 1] + 4 * seq_cache[k - 2]
                  + 5 * seq_cache[k - 5] + 4 * seq_cache[k - 6];
        seq_t neg = 11 * seq_cache[k - 3] + 6 * seq_cache[k - 4];
        seq_cache[k] = pos - neg;   /* guaranteed non‑negative for this sequence */
    }

    seq_cache_size = n;
    return seq_cache[n];
}
