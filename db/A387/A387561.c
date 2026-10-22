/*
 * A387561  Number of independent vertex sets in the n-Cameron graph.
 * https://oeis.org/A387561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1)-16*a(n-2)-306*a(n-3)-27*a(n-4)+12*a(n-5);
 *            Initial values: a(1)=75, a(2)=2499, a(3)=83079, a(4)=2761659,
 *            a(5)=91800447
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store computed terms.  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;     /* highest index that has been computed */

/* Ensure that cache contains terms up to n.  Compute missing terms using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    idx_t new_size = n + 1;           /* allocate space up to index n (inclusive) */

    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
    } else if (new_size > cache_size + 1) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    }

    idx_t start = cache_size + 1;
    for (idx_t i = start; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)75;
        else if (i == 2)
            cache[i] = (seq_t)2499;
        else if (i == 3)
            cache[i] = (seq_t)83079;
        else if (i == 4)
            cache[i] = (seq_t)2761659;
        else if (i == 5)
            cache[i] = (seq_t)91800447;
        else {
            /* Compute a(i) using the recurrence.  The order guarantees
               non‑negative intermediate values for all valid n. */
            seq_t part1 = 34 * cache[i-1] + 12 * cache[i-5];
            seq_t part2 = 16 * cache[i-2] + 306 * cache[i-3] + 27 * cache[i-4];
            cache[i] = part1 - part2;
        }
    }

    cache_size = n;
}

/* Public function: returns the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset, return 0 as a neutral value */

    ensure_cache(n);
    return cache[n];
}
