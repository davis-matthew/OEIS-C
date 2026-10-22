/*
 * A373911  Number of compositions of 7*n into parts 5 and 7.
 * https://oeis.org/A373911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1)-21*a(n-2)+35*a(n-3)-35*a(n-4)+22*a(n-5)-7*a(n-6)+a(n-7)
   Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=9 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* dynamically growable array */
    static size_t cache_cap = 0;     /* allocated capacity (elements) */
    static idx_t cache_max  = (idx_t)-1;   /* highest index whose value is valid */

    /* initialise cache with the first seven values */
    if (cache == NULL) {
        cache_cap = 7;                     /* we need indices 0..6 at least */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure, return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)2;
        cache[6] = (seq_t)9;
        cache_max = 6;
    }

    /* grow cache if larger n is requested */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* if we have already computed up to n, just return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* compute any missing values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val = 7 * cache[i - 1];
        val -= 21 * cache[i - 2];
        val += 35 * cache[i - 3];
        val -= 35 * cache[i - 4];
        val += 22 * cache[i - 5];
        val -= 7 * cache[i - 6];
        val += cache[i - 7];
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
