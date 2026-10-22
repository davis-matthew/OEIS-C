/*
 * A384853  Squared length of interior diagonal of n-th (U, V)-crossbox, where
 *          U = (1, 0, 1) and V = (0, 1, 0), as in Comments.
 * https://oeis.org/A384853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=5, a(3)=9; for n>=4, a(n)=4*a(n-1)-3*a(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula follows from a(n)=1+2*(3^(n-1)+1) for n≥2, with the OEIS
 * offset of 1.  A simple recurrence is used for efficiency and memoisation
 * ensures any order of calls works correctly. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is 1‑based (offset 1).  Values for n>=1 are required. */
    static seq_t *cache = NULL;   /* array of a(1) .. a(cache_len) */
    static idx_t cache_len = 0;   /* number of elements stored */

    /* Expand cache to hold at least n elements.  (size_t cast is safe
     * for the small n values of interest.) */
    if (n > cache_len) {
        size_t new_cap = (size_t)n;          /* new capacity in elements */
        if (!cache) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!cache) return (seq_t)0;     /* allocation failure, unlikely */
        } else {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
        }
    }

    /* Initialise first terms if not already done. */
    if (cache_len < 1) {            /* a(1)=1 */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len < 2 && n >= 2) {  /* a(2)=5 */
        cache[1] = (seq_t)5;
        cache_len = 2;
    }
    if (cache_len < 3 && n >= 3) {  /* a(3)=9 */
        cache[2] = (seq_t)9;
        cache_len = 3;
    }

    /* Compute remaining terms up to n using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t prev1 = cache[k-2];   /* a(k-1) */
        seq_t prev2 = cache[k-3];   /* a(k-2) */
        cache[k-1] = (seq_t)4 * prev1 - (seq_t)3 * prev2;
    }
    cache_len = n;

    return cache[n-1];
}
