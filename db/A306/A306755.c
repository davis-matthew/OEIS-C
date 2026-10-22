/*
 * A306755  a(n) = a(n-6) + a(n-7) with a(0)=7, a(1)=...=a(5)=0, a(6)=6.
 * https://oeis.org/A306755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-6) + a(n-7); a(0)=7, a(1)=a(2)=a(3)=a(4)=a(5)=0,
 *            a(6)=6
 * Range:     u8 n <= 50, u16 n <= 102, u32 n <= 207, u64 n <= 415,
 *            u128 n <= 830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoisation array */
static idx_t  cache_size = 0;        /* allocated length of cache   */
static idx_t  last_computed = 0;     /* largest n that has been cached */

/* Allocate/extend the cache up to at least n, then fill missing values. */
static void ensure_cache(idx_t n)
{
    if (n < 7)          /* we don't cache values below 7 */
        return;

    if (cache_size <= n) {
        idx_t newsize = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = newsize;
    }

    /* Initialise base values if not yet stored. */
    if (last_computed < 6) {
        cache[0] = (seq_t)7;
        cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = (seq_t)0;
        cache[6] = (seq_t)6;
        last_computed = 6;
    }

    for (idx_t i = last_computed + 1; i <= n; ++i)
        cache[i] = cache[i - 6] + cache[i - 7];

    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 7) {                 /* small indices handled directly */
        switch (n) {
            case 0: return (seq_t)7;
            case 1: case 2: case 3: case 4: case 5: return (seq_t)0;
            case 6: return (seq_t)6;
        }
    }

    ensure_cache(n);
    return cache[n];
}
