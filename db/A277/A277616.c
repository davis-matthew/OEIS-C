/*
 * A277616  Lexicographically earliest sequence such that |a(n+1)-a(n)| is a
 *          square > 1 (for all n) and no number occurs twice; a(0) = 0.
 * https://oeis.org/A277616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277616:
   Lexicographically earliest sequence such that |a(n+1)-a(n)| is a square >1,
   no repeats, a(0)=0.
   Recurrence:  a(n) = a(n-1) + a(n-13) - a(n-14)   for n > 13,
   with initial values a(0..13) given by the OEIS entry.
*/

#include <stdlib.h>

static const seq_t init_vals[14] = {
    (seq_t)0,  (seq_t)4, (seq_t)8, (seq_t)12,
    (seq_t)3,  (seq_t)7, (seq_t)11, (seq_t)2,
    (seq_t)6,  (seq_t)10, (seq_t)1, (seq_t)5,
    (seq_t)9,  (seq_t)13
};

static seq_t  *cache  = NULL;   /* memoisation array */
static size_t  cap    = 0;      /* allocated capacity */
static idx_t   len    = 0;      /* number of values stored */

/* Initialise the cache with the first 14 terms. */
static void
init_cache(void)
{
    cap = 14;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    for (idx_t i = 0; i < 14; ++i)
        cache[i] = init_vals[i];
    len = 14;
}

/* Ensure that values up to index n are computed and stored in cache. */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL)
        init_cache();

    /* Grow the capacity if necessary. */
    if ((idx_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;                    /* double until big enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing values using the recurrence. */
    if (len <= n) {
        idx_t start = len > 14 ? len : 14;
        for (idx_t k = start; k <= n; ++k) {
            /* a(k) = a(k-1) + a(k-13) - a(k-14)   (for k >= 14) */
            cache[k] = cache[k - 1] + cache[k - 13] - cache[k - 14];
        }
        len = n + 1;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
