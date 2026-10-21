/*
 * A264484  Number of (n+1) X (1+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 2,-1 1,0 2,1 0,-1 -2,-2
 *          or -1,0.
 * https://oeis.org/A264484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) + 4*a(n-5) + a(n-6) -
 *            a(n-9); a(1)=1, a(2)=3, a(3)=4, a(4)=12, a(5)=25, a(6)=52,
 *            a(7)=121, a(8)=261, a(9)=576
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised cache of computed terms: cache[i] == a(i+1) */
static seq_t *cache = NULL;
static size_t cached = 0;   /* number of terms currently cached */

/* Initial nine values */
static const seq_t init_vals[9] = {
    (seq_t)1,  (seq_t)3,  (seq_t)4,
    (seq_t)12, (seq_t)25, (seq_t)52,
    (seq_t)121, (seq_t)261, (seq_t)576
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    /* Extend cache if necessary */
    if (cached < (size_t)n) {
        idx_t new_cap = n;                /* number of terms required */
        /* realloc may return a void*; cast to seq_t* for C++ compatibility */
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
        /* If realloc fails, cache will be NULL; the caller will get a NUL
           dereference.  The problem statement does not require error handling
           beyond basic allocation. */
        size_t i;
        for (i = cached; i < (size_t)new_cap; ++i) {
            if (i < 9) {
                cache[i] = init_vals[i];
            } else {
                /* Compute a(i+1) using the recurrence */
                cache[i] =
                    (seq_t)2 * cache[i-1]          /* 2*a(n-1) */
                  + cache[i-3]                     /* + a(n-3) */
                  - (seq_t)2 * cache[i-4]          /* - 2*a(n-4) */
                  + (seq_t)4 * cache[i-5]          /* + 4*a(n-5) */
                  + cache[i-6]                     /* + a(n-6) */
                  - cache[i-9];                    /* - a(n-9) */
            }
        }
        cached = (size_t)new_cap;
    }

    return cache[n-1];    /* cache is 0‑based; a(n) is at index n-1 */
}
