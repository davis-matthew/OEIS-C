/*
 * A398061  Upper (1/5,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) with
 *            initial values (a(0),...,a(6)) = (0, 2, 5, 11, 18, 28, 41)
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding computed terms   */
    static size_t cap   = 0;       /* capacity of the array          */
    static size_t len   = 0;       /* number of terms stored (0..len-1) */

    /* establish the first 7 terms once */
    if (len == 0) {
        cap  = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;  cache[1] = 2;  cache[2] = 5;
        cache[3] = 11; cache[4] = 18; cache[5] = 28;
        cache[6] = 41;
        len = 7;
    }

    /* extend cache if necessary */
    size_t target = (size_t)n;
    if (target >= len) {
        /* enlarge array to hold up to target */
        if (cap <= target) {
            size_t new_cap = cap ? cap : 8;
            while (new_cap <= target) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = new_cap;
        }

        /* compute terms from current len up to target */
        for (size_t i = len; i <= target; ++i) {
            seq_t term = cache[i-1];
            term *= 2;                 /* 2*a(i-1) */
            term -= cache[i-2];         /* - a(i-2) */
            term += cache[i-5];         /* + a(i-5) */
            term -= 2 * cache[i-6];     /* - 2*a(i-6) */
            term += cache[i-7];         /* + a(i-7) */
            cache[i] = term;
        }
        len = target + 1;
    }

    return cache[(size_t)n];
}
