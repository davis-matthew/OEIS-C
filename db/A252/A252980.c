/*
 * A252980  Number of n X 5 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          increasing by 0 or 1 with every step right, diagonally se or down.
 * https://oeis.org/A252980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n>11; Initial values:
 *            a(1)=6, a(2)=41, a(3)=266, a(4)=1247, a(5)=4657, a(6)=67537,
 *            a(7)=433401, a(8)=1481460, a(9)=3510600, a(10)=6637020,
 *            a(11)=10878576
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 94, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static const seq_t init_vals[11] = {
    6, 41, 266, 1247, 4657, 67537, 433401,
    1481460, 3510600, 6637020, 10878576
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safety; not used for n>=1 */

    /* Initialize cache with the first 11 terms if not yet done */
    if (cache_len < 11) {
        idx_t need = 11;
        if (!cache) {
            cache = (seq_t *)malloc(need * sizeof(seq_t));
        } else if (cache_len < need) {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        }
        for (idx_t i = cache_len; i < need; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = need;
    }

    /* Return cached value if already computed */
    if (n <= cache_len) {
        return cache[n-1];
    }

    /* Extend cache up to index n */
    idx_t new_len = n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    for (idx_t i = cache_len; i < new_len; ++i) {
        /* recurrence: a(i+1) = 3*a(i) - 3*a(i-1) + a(i-2) */
        cache[i] = (3 * cache[i-1]) - (3 * cache[i-2]) + cache[i-3];
    }
    cache_len = new_len;
    return cache[n-1];
}
