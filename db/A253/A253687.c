/*
 * A253687  The total number of pentagons in a variant of pentagon expansion
 *          (side-to-side, two consecutive sides and one isolated side) after
 *          n iterations.
 * https://oeis.org/A253687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)-a(n-2)+a(n-4)-2*a(n-5)+a(n-6) for n>7 Initial
 *            values: a(1)=1, a(2)=4, a(3)=10, a(4)=21, a(5)=39, a(6)=64,
 *            a(7)=94, a(8)=129, a(9)=167, a(10)=212
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1 */

    /* static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t   cache_size = 0;       /* number of terms stored (a(1)..a(cache_size)) */
    static size_t  cache_cap   = 0;       /* allocated capacity */

    /* initialise cache with first ten terms */
    if (cache == NULL) {
        cache_cap = 10;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();

        cache[0] = (seq_t)1;    /* a(1) */
        cache[1] = (seq_t)4;    /* a(2) */
        cache[2] = (seq_t)10;   /* a(3) */
        cache[3] = (seq_t)21;   /* a(4) */
        cache[4] = (seq_t)39;   /* a(5) */
        cache[5] = (seq_t)64;   /* a(6) */
        cache[6] = (seq_t)94;   /* a(7) */
        cache[7] = (seq_t)129;  /* a(8) */
        cache[8] = (seq_t)167;  /* a(9) */
        cache[9] = (seq_t)212;  /* a(10) */
        cache_size = 10;
    }

    /* enlarge capacity if needed */
    if (n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    if (n > cache_size) {
        for (idx_t k = cache_size + 1; k <= n; ++k) {
            /* a(k) = 2*a(k-1)-a(k-2)+a(k-4)-2*a(k-5)+a(k-6) */
            seq_t val =
                (seq_t)2 * cache[k - 2]
              - cache[k - 3]
              + cache[k - 5]
              - (seq_t)2 * cache[k - 6]
              + cache[k - 7];
            cache[k - 1] = val;
        }
        cache_size = n;
    }

    return cache[n - 1];   /* cache[0] holds a(1) */
}
