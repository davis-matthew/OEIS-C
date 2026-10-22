/*
 * A398657  Lower (1, 3/2) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6), * with (a(0), a(1), a(2), a(3), a(4), a(5)) = (0, 2, 10,
 *            27, 55, 97)
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* current capacity of cache */
    static idx_t last  = (idx_t)-1; /* highest n stored, use sentinel */

    /* initialise cache with first 6 terms on first call */
    if (cache == NULL) {
        cap  = 6;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)27;
        cache[4] = (seq_t)55;
        cache[5] = (seq_t)97;
        last = 5;
    }

    /* If requested value is already computed, return it */
    if (n <= last) {
        return cache[n];
    }

    /* Expand cache to accommodate up to n */
    if (n >= cap) {
        idx_t newcap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out of memory, should never happen here */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute new values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* Long addition: avoid intermediate underflow by splitting positive and negative parts */
        seq_t pos = cache[i-1] * (seq_t)4
                  + cache[i-3] * (seq_t)8
                  + cache[i-5] * (seq_t)4;
        seq_t neg = cache[i-2] * (seq_t)7
                  + cache[i-4] * (seq_t)7
                  + cache[i-6];
        cache[i] = pos - neg; /* pos >= neg for all terms, safe unsigned subtraction */
    }
    last = n;
    return cache[n];
}
