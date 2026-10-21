/*
 * A252954  Number of (n+2)X(1+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 24, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) -10*a(n-2) +10*a(n-3) -61*a(n-4) +124*a(n-5) 
 *              +45*a(n-6) -182*a(n-7) -54*a(n-8) -570*a(n-9) +2205*a(n-10) 
 *              -2268*a(n-11) +756*a(n-12)   for n > 15
 * Initial values (n = 1 .. 15):
 * 60, 140, 297, 711, 1543, 3461, 7637, 16689, 35609, 78429, 165847,
 * 353699, 759497, 1602963, 3361709
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* zero index is unused; return 0 for n==0 (outside the OEIS offset) */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_last = 0;     /* highest index computed */

    size_t idx = (size_t)n;

    /* initialise cache with the first 15 terms on first call */
    if (cache == NULL) {
        /* reserve space for indices 0..15 */
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            exit(EXIT_FAILURE);   /* allocation failure */

        /* initial values (index 1 through 15) */
        cache[1]  = (seq_t)60;      cache[2]  = (seq_t)140;
        cache[3]  = (seq_t)297;      cache[4]  = (seq_t)711;
        cache[5]  = (seq_t)1543;     cache[6]  = (seq_t)3461;
        cache[7]  = (seq_t)7637;     cache[8]  = (seq_t)16689;
        cache[9]  = (seq_t)35609;    cache[10] = (seq_t)78429;
        cache[11] = (seq_t)165847;   cache[12] = (seq_t)353699;
        cache[13] = (seq_t)759497;   cache[14] = (seq_t)1602963;
        cache[15] = (seq_t)3361709;

        cache_last = 15;
    }

    /* extend cache if needed */
    if (idx > cache_last) {
        /* enlarge the storage */
        if (idx + 1 > cache_cap) {
            size_t new_cap = (idx + 1 > cache_cap * 2) ? (idx + 1) : (cache_cap * 2);
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms from cache_last+1 up to idx */
        for (size_t i = cache_last + 1; i <= idx; ++i) {
            if (i <= 15) /* already initialized */ 
                continue;

            /* compute with separate positive and negative parts
               to avoid underflow when using unsigned seq_t */
            seq_t pos = (seq_t)0;
            pos += (seq_t)6   * cache[i-1];
            pos += (seq_t)10  * cache[i-3];
            pos += (seq_t)124 * cache[i-5];
            pos += (seq_t)45  * cache[i-6];
            pos += (seq_t)2205* cache[i-10];
            pos += (seq_t)756 * cache[i-12];

            seq_t neg = (seq_t)0;
            neg += (seq_t)10  * cache[i-2];
            neg += (seq_t)61  * cache[i-4];
            neg += (seq_t)182 * cache[i-7];
            neg += (seq_t)54  * cache[i-8];
            neg += (seq_t)570 * cache[i-9];
            neg += (seq_t)2268* cache[i-11];

            cache[i] = pos - neg; /* pos >= neg for all n>15 */
        }
        cache_last = idx;
    }

    return cache[idx];
}
