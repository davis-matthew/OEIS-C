/*
 * A284699  Number of dominating sets in the n-antiprism graph.
 * https://oeis.org/A284699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + a(n-3) + a(n-4) + a(n-5) (n>5)
 *            Initial values: a(1)=3, a(2)=15, a(3)=57, a(4)=223, a(5)=863
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cache[0] corresponds to a(1) */
static idx_t cache_len = 0;     /* number of values stored in cache */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >=1 in the problem statement.
       For safety, return 0 for n==0, though it is unused. */
    if (n == 0) return (seq_t)0;

    /* Extend cache to n if necessary */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed – unlikely */
        cache = new_cache;

        /* Compute values up to new_len */
        for (idx_t i = cache_len + 1; i <= new_len; ++i) {
            switch (i) {
                case 1: cache[0] = (seq_t)3;   break;
                case 2: cache[1] = (seq_t)15;  break;
                case 3: cache[2] = (seq_t)57;  break;
                case 4: cache[3] = (seq_t)223; break;
                case 5: cache[4] = (seq_t)863; break;
                default:
                    cache[i-1] = (seq_t)(
                        3 * cache[i-2] +          /* 3*a(i-1) */
                        3 * cache[i-3] +          /* 3*a(i-2) */
                        cache[i-4] +              /* a(i-3)   */
                        cache[i-5] +              /* a(i-4)   */
                        cache[i-6]);              /* a(i-5)   */
                    break;
            }
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
