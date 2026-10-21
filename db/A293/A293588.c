/*
 * A293588  E.g.f.: exp(x + x^6/6).
 * https://oeis.org/A293588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 15, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A293588:  a(0)=a(1)=…=a(5)=1
   a(n) = a(n-1) + ((n-1)!/(n-6)!)*a(n-6)   (recursive formula)   */

#include <stdlib.h>   /* for malloc / realloc */

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) up to the current capacity. */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static size_t  cap   = 0;     /* number of terms already computed */

    /* For small n, the answer is 1 and we may avoid allocating. */
    if (n < 6) return (seq_t)1;

    /* Ensure the cache is large enough for index n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp;

        if (cache == NULL) {                    /* first allocation */
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {                                 /* grow the array */
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (!tmp) return (seq_t)0;  /* allocation failure – return 0 */

        /* Compute any new terms from old cap up to new_cap-1. */
        for (size_t i = cap; i < new_cap; ++i) {
            if (i < 6) {
                tmp[i] = (seq_t)1;   /* base values */
            } else {
                /* factorial ratio (i-1)!/(i-6)! = (i-1)*(i-2)*…*(i-5) */
                seq_t prod = (seq_t)1;
                for (int k = 1; k <= 5; ++k)
                    prod *= (seq_t)(i - k);

                tmp[i] = tmp[i - 1] + prod * tmp[i - 6];
            }
        }

        cache = tmp;   /* install the (possibly reallocated) array */
        cap   = new_cap;
    }

    return cache[(size_t)n];
}
