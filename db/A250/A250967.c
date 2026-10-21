/*
 * A250967  Number of (n+1) X (2+1) 0..1 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A250967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 13*a(n-2) + 17*a(n-3) - 14*a(n-4) + 3*a(n-5)
 *            with a(1)=49, a(2)=308, a(3)=1945, a(4)=12281, a(5)=77537
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache          = NULL;   /* 0‑based storage of computed terms */
static size_t cache_len      = 0;      /* number of terms already computed   */
static size_t cache_cap      = 0;      /* allocated capacity in elements      */

/* compute and return a(n) for n>=1 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined, but guard */

    /* initialise or extend storage as needed */
    if (cache == NULL) {
        cache_cap = 8;                      /* start with a reasonable size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);                /* allocation failure */
    }

    /* grow capacity until we can store up to n terms */
    while (cache_len < (size_t)n) {
        if (cache_len == cache_cap) {       /* need more room */
            size_t new_cap = cache_cap << 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute the next term (index cache_len -> a(cache_len+1)) */
        seq_t val;
        if (cache_len == 0)                /* a(1) */
            val = (seq_t)49;
        else if (cache_len == 1)           /* a(2) */
            val = (seq_t)308;
        else if (cache_len == 2)           /* a(3) */
            val = (seq_t)1945;
        else if (cache_len == 3)           /* a(4) */
            val = (seq_t)12281;
        else if (cache_len == 4)           /* a(5) */
            val = (seq_t)77537;
        else {
            /* recurrence: a(n) = 8*a(n-1) - 13*a(n-2) + 17*a(n-3)
                 - 14*a(n-4) + 3*a(n-5)              */
            val = 8  * cache[cache_len-1]          /* a(n-1)   */
                - 13 * cache[cache_len-2]          /* a(n-2)   */
                + 17 * cache[cache_len-3]          /* a(n-3)   */
                - 14 * cache[cache_len-4]          /* a(n-4)   */
                + 3  * cache[cache_len-5];         /* a(n-5)   */
        }

        cache[cache_len] = val;                /* store computed term */
        cache_len++;                          /* increment computed count */
    }

    return cache[n-1];                         /* return a(n) */
}
