/*
 * A287381  a(n) = a(n-1) + 2*a(n-2) - a(n-3), where a(0) = 2, a(1) = 4, a(2)
 *          = 7.
 * https://oeis.org/A287381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 73, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + 2*a(n-2) - a(n-3),
   a(0) = 2, a(1) = 4, a(2) = 7 */

#include <stdlib.h>   /* for malloc / realloc / abort */

static seq_t *cache = NULL;   /* memoisation array */
static size_t len   = 0;      /* number of computed elements   */
static size_t cap   = 0;      /* capacity of the cache         */

/* Ensure that the cache can hold at least 'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (cap < needed) {
        size_t newcap = cap ? cap << 1 : 1;
        while (newcap < needed)
            newcap <<= 1;
        /* cast to seq_t* so the code compiles under C++ */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cap   = newcap;
    }
}

/* Sequence entry point. */
seq_t
a(idx_t n)
{
    /* initialise seed values once */
    if (len == 0) {
        ensure_capacity(3);
        cache[0] = 2;
        cache[1] = 4;
        cache[2] = 7;
        len = 3;
    }

    /* already known? */
    if ((idx_t)n < (idx_t)len)
        return cache[n];

    /* extend memoisation up to n */
    ensure_capacity((size_t)n + 1);
    for (size_t i = len; i <= (size_t)n; ++i) {
        /* recurrence: a(i) = a(i-1) + 2*a(i-2) - a(i-3) */
        cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] - cache[i-3];
    }
    len = (size_t)n + 1;   /* update known length */
    return cache[n];
}
