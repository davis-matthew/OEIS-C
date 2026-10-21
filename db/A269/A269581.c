/*
 * A269581  Number of length-n 0..6 arrays with no repeated value differing
 *          from the previous repeated value by more than one.
 * https://oeis.org/A269581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 30*a(n-1) - 356*a(n-2) + 2064*a(n-3) - 5618*a(n-4) + 4356*a(n-5) + 4964*a(n-6) + 480*a(n-7) - 143*a(n-8) - 6*a(n-9)
   Initial values:
   a(1) = 7
   a(2) = 49
   a(3) = 343
   a(4) = 2371
   a(5) = 16237
   a(6) = 110339
   a(7) = 745013
   a(8) = 5003189
   a(9) = 33444515
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the n-th term (n >= 1) of OEIS A269581. */
    /* The first 9 terms are hard‑coded. For n > 9 we use the
       9‑term recurrence and keep a static cache so that repeated
       calls are fast. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms (cache[0] = a(1)) */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Handle the trivial case */
    if (n <= 9)
    {
        static const seq_t init[9] = {
            (seq_t)7, (seq_t)49, (seq_t)343, (seq_t)2371,
            (seq_t)16237, (seq_t)110339, (seq_t)745013,
            (seq_t)5003189, (seq_t)33444515
        };
        return init[n - 1];
    }

    /* Ensure we have a cache that contains the first 9 terms. */
    if (cache_size == 0)
    {
        size_t init_cap = (n > 9 ? n : 10);
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_cap = init_cap;
        cache[0] = (seq_t)7;
        cache[1] = (seq_t)49;
        cache[2] = (seq_t)343;
        cache[3] = (seq_t)2371;
        cache[4] = (seq_t)16237;
        cache[5] = (seq_t)110339;
        cache[6] = (seq_t)745013;
        cache[7] = (seq_t)5003189;
        cache[8] = (seq_t)33444515;
        cache_size = 9;
    }

    /* Resize the cache if necessary (grow to at least n). */
    if (cache_cap < n)
    {
        size_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n using the recurrence. */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        /* indices in cache are zero‑based: a(k) -> cache[k-1] */
        seq_t t = 30 * cache[i - 2]          /* 30*a(i-1)          */
                + 2064 * cache[i - 4]        /* +2064*a(i-3)        */
                + 4356 * cache[i - 6]        /* +4356*a(i-5)        */
                + 4964 * cache[i - 7]        /* +4964*a(i-6)        */
                + 480  * cache[i - 8];       /* + 480*a(i-7)        */

        t -= 356 * cache[i - 3];            /* -356*a(i-2)         */
        t -= 5618 * cache[i - 5];           /* -5618*a(i-4)        */
        t -= 143  * cache[i - 9];           /* -143*a(i-8)         */
        t -= 6    * cache[i - 10];          /* -6*a(i-9)           */

        cache[i - 1] = t;
    }
    cache_size = n;
    return cache[n - 1];
}
