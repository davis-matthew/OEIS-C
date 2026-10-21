/*
 * A262018  The first of five consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of eleven
 *          consecutive positive integers.
 * https://oeis.org/A262018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 179*a(n-1)-179*a(n-2)+a(n-3) Initial values: a(1)=28,
 *            a(2)=5308, a(3)=945148 Offset: 1 (a(1) is the first term)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    /* n < 1 is outside the intended offset; return 0 */
    if (n < 1) return 0;

    /* increase cache capacity if needed */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;      /* allocate indices 0..n */
        seq_t *tmp;

        if (cache_len == 0) {
            /* first allocation */
            tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!tmp) return 0;                /* allocation failed */
            cache = tmp;
            cache[0] = (seq_t)0;                 /* placeholder at index 0 */
            cache_len = 1;
        } else {
            tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return 0;                /* allocation failed */
            cache = tmp;
        }

        /* compute missing values */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 1) {
                cache[i] = (seq_t)28;
            } else if (i == 2) {
                cache[i] = (seq_t)5308;
            } else if (i == 3) {
                cache[i] = (seq_t)945148;
            } else {
                seq_t t = (seq_t)179 * cache[i-1];
                t -= (seq_t)179 * cache[i-2];
                t += cache[i-3];
                cache[i] = t;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
