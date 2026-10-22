/*
 * A398056  Lower (1,4/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7);
   Initial values: a(0)=0, a(1)=1, a(2)=6, a(3)=13, a(4)=22, a(5)=35, a(6)=49. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of computed terms. The cache is grown as needed
       and filled sequentially so that calls in any order are O(1) per
       new term. */
    static seq_t *cache = NULL;
    static idx_t sz = 0;          /* number of stored terms */
    static idx_t cap = 0;         /* allocated capacity */

    /* Ensure initial terms are present */
    if (sz == 0) {
        cap = 8;   /* at least 7 initial terms + a dummy placeholder */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)22;
        cache[5] = (seq_t)35;
        cache[6] = (seq_t)49;
        sz = 7;
    }

    /* Expand cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n >= sz) {
        for (idx_t i = sz; i <= n; ++i) {
            /* recurrence implementation */
            seq_t val = 2 * cache[i - 1]
                       - cache[i - 2]
                       + cache[i - 5]
                       - 2 * cache[i - 6]
                       + cache[i - 7];
            cache[i] = val;
        }
        sz = n + 1;
    }

    return cache[(size_t)n];
}
