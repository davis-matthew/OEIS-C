/*
 * A251672  8-step Fibonacci sequence starting with 0,0,0,0,0,0,1,0.
 * https://oeis.org/A251672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 72, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  8‑step Fibonacci sequence
    a(n+8) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6)+a(n+7)
    initial terms: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=1, a(7)=0
    The implementation caches computed values so that each term is
    evaluated at most once, regardless of call order.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached entries */

    /* initialise the cache with the first eight terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 8);
        if (cache == NULL) return 0;           /* allocation failure      */
        cache_size = 8;
        cache[0] = cache[1] = cache[2] = cache[3] = cache[4] = cache[5] = (seq_t)0;
        cache[6] = (seq_t)1;
        cache[7] = (seq_t)0;
    }

    /* extend cache if the requested index is beyond the cached domain */
    if (n >= cache_size) {
        /* reallocate to fit indexes up to n */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) return 0;             /* allocation failure      */
        cache = tmp;

        /* compute missing terms starting from the first missing index */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            /* i >= 8 holds because cache_size is at least 8 */
            for (idx_t k = 0; k < 8; ++k) {
                sum += cache[i - 8 + k];
            }
            cache[i] = sum;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
