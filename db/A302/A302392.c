/*
 * A302392  Number of odd parts in the partitions of 3n into 3 parts.
 * https://oeis.org/A302392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) with
 *            initial values a(1)=3, a(2)=4, a(3)=13, a(4)=18, a(5)=33,
 *            a(6)=40, a(7)=61
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc/realloc */

static seq_t *cache = NULL;         /* 1‑based array of computed values; cache[0] unused */
static idx_t  cache_last = 0;       /* largest index that has been computed */
static idx_t  cache_cap  = 0;       /* current allocation size */

/* Ensure the cache can hold entries up to index n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    idx_t needed = n + 1;           /* indices 0..n inclusive */
    if (needed <= cache_cap) return;

    if (cache == NULL)
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));

    cache_cap = needed;
}

/* Compute a(n) for n >= 1.  Memoized, works in any call order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n==0; return 0 safely */

    /* initialize base values once */
    if (cache_last == 0) {
        ensure_cache_size(7);
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)18;
        cache[5] = (seq_t)33;
        cache[6] = (seq_t)40;
        cache[7] = (seq_t)61;
        cache_last = 7;
    }

    /* grow the storage if needed */
    ensure_cache_size(n);

    /* compute any missing terms up to n */
    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t val =  cache[i-1];
            val += cache[i-2];
            val -= cache[i-3];
            val += cache[i-4];
            val -= cache[i-5];
            val -= cache[i-6];
            val += cache[i-7];
            cache[i] = val;
        }
        cache_last = n;
    }

    return cache[n];
}
