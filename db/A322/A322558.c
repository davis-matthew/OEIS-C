/*
 * A322558  a(0)=1, a(1)=1; for n>1, a(n)=a(n-1)+a(n-2) if a(n-1) is less than
 *          or equal to n-1, otherwise a(n)=a(n-1)-(n-1).
 * https://oeis.org/A322558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A322558:  a(0)=1, a(1)=1; for n>1,
   a(n)=a(n-1)+a(n-2) if a(n-1)<=n-1, otherwise a(n)=a(n-1)-(n-1). */

seq_t a(idx_t n)
{
    /* Use a static cache so repeated calls are fast and order‑independent. */
    static seq_t *cache      = NULL;
    static size_t  capacity  = 0;     /* number of allocated elements */
    static size_t  max_comp  = (size_t)-1; /* largest index with a computed value */

    size_t idx = (size_t)n;

    /* Grow the cache to hold at least idx+1 elements. */
    if (cache == NULL) {
        capacity = idx + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;          /* allocation failure */
    } else if (idx + 1 > capacity) {
        capacity = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;          /* allocation failure */
        cache = tmp;
    }

    /* Initialise the first two terms if this is the first call. */
    if (max_comp == (size_t)-1) {
        cache[0] = (seq_t)1;
        if (capacity > 1) cache[1] = (seq_t)1;
        max_comp = capacity > 1 ? 1 : 0;
    }

    /* Compute terms up to idx if necessary. */
    for (size_t i = max_comp + 1; i <= idx; ++i) {
        /* a(i) depends on a(i-1) and a(i-2) */
        seq_t prev   = cache[i - 1];
        seq_t prev2  = cache[i - 2];
        seq_t cmp_val = (seq_t)(i - 1);

        if (prev <= cmp_val)
            cache[i] = prev + prev2;
        else
            cache[i] = prev - cmp_val;
    }
    max_comp = idx;

    return cache[idx];
}
