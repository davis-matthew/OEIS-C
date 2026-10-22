/*
 * A398196  Lower (2/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) with
 *            initial values a(0)=0, a(1)=1, a(2)=5, a(3)=11, a(4)=20,
 *            a(5)=31, a(6)=44
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the n-th term of A398196 (lower (2/5,1) midsequence) */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static idx_t size = 0;           /* number of elements stored (next free index) */
    static idx_t cap  = 0;           /* allocated capacity */

    /* initialize cache with first seven terms on first call */
    if (size == 0) {
        /* start with a small capacity, grow as needed */
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(*cache));
        /* initial terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)20;
        cache[5] = (seq_t)31;
        cache[6] = (seq_t)44;
        size = 7;           /* indices 0..6 are now valid */
    }

    /* if the requested index is already computed, return it */
    if (n < size)
        return cache[n];

    /* grow the cache to hold up to index n */
    if (cap <= n) {
        size_t new_cap = (cap > 0) ? cap * 2 : 8;
        while (new_cap <= n)
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(*cache));
        cap = (idx_t)new_cap;
    }

    /* compute all missing terms up to n, using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        if (i < 7) {
            /* safety: the first seven indices are already set during init,
               but the loop may reach them in the off‑case where a small n
               is requested after a larger n was already computed. */
            /* No action needed */
            continue;
        }

        /* recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) */
        seq_t val = 2 * cache[i - 1]
                  - cache[i - 2]
                  + cache[i - 5]
                  - 2 * cache[i - 6]
                  + cache[i - 7];
        cache[i] = val;
    }

    size = n + 1;
    return cache[n];
}
