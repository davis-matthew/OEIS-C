/*
 * A398055  Upper (1,3/5) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A398055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) with
 *            initial values: a(0)=0, a(1)=2, a(2)=6, a(3)=12, a(4)=20,
 *            a(5)=30, a(6)=43
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* initialise cache with the first 7 terms */
    if (cap == 0) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) return 0;                  /* graceful failure */
        cache[0] = 0;   cache[1] = 2;   cache[2] = 6;
        cache[3] = 12;  cache[4] = 20;  cache[5] = 30;
        cache[6] = 43;
        cap = 7;
    }

    /* grow the cache up to n */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* graceful failure */
        cache = tmp;

        for (size_t i = cap; i <= n; ++i) {
            /* compute a[i] = 2*a[i-1] - a[i-2] + a[i-5] - 2*a[i-6] + a[i-7] */
            seq_t pos = cache[i-1] + cache[i-1];  /* 2*a[i-1]   */
            pos += cache[i-5];                   /* + a[i-5]    */
            pos += cache[i-7];                   /* + a[i-7]    */

            seq_t neg = cache[i-2];              /* - a[i-2]    */
            neg += cache[i-6];                   /* - 2*a[i-6]  */
            neg += cache[i-6];

            cache[i] = pos - neg;                /* positive - negative */
        }
        cap = new_cap;
    }

    return cache[n];
}
