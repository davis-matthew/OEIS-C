/*
 * A365538  a(0) = 1; otherwise, for i >= 0, a(4i+0) = a(4i+1) = a(2i),
 *          a(4i+2) = 2*a(2i+1), a(4i+3) = 0.
 * https://oeis.org/A365538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(0)=1; for i≥0: a(4i)=a(4i+1)=a(2i), a(4i+2)=2*a(2i+1),
 *            a(4i+3)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to remember values for any call order */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize base case */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));  /* size 1 for a[0] */
        if (cache == NULL) return (seq_t)0; /* memory failure */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache if needed */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        if (n + 1 > cache_len) {
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * (n + 1));
            if (tmp == NULL) return (seq_t)0;      /* memory failure */
            cache = tmp;
            cache_len = n + 1;
        }
        /* fill missing entries */
        for (idx_t i = old_len; i <= n; ++i) {
            idx_t h = i >> 1;                 /* i/2 */
            if ((i & 3) == 3) {               /* i % 4 == 3 */
                cache[i] = (seq_t)0;
            } else if ((i & 3) == 2) {         /* i % 4 == 2 */
                cache[i] = cache[h] * (seq_t)2;
            } else {                           /* i % 4 == 0 or 1 */
                cache[i] = cache[h];
            }
        }
    }
    return cache[n];
}
