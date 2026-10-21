/*
 * A275230  Number of 5 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 9*a(n-2) - 15*a(n-3) + 20*a(n-4) - 6*a(n-5) ,
 *            n > 6 Initial values: a(1) = 8 a(2) = 66 a(3) = 648 a(4) = 6448
 *            a(5) = 64248 a(6) = 640250
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t cur_len = 0;          /* number of computed terms */

    /* ensure we have capacity for at least n terms, but at least 6 for init */
    size_t need = (size_t)n;
    if (need < 6) need = 6;
    if (need > cap) {
        seq_t *new_arr = (seq_t *)malloc(need * sizeof(seq_t));
        if (!new_arr) exit(1);          /* memory allocation failed */
        if (cache) {
            memcpy(new_arr, cache, cur_len * sizeof(seq_t));
            free(cache);
        }
        cache = new_arr;
        cap   = need;
    }

    /* initialise the first six terms if not already initialised */
    if (cur_len < 6) {
        const seq_t init[6] = { (seq_t)8,   (seq_t)66,
                                (seq_t)648, (seq_t)6448,
                                (seq_t)64248, (seq_t)640250 };
        size_t start = cur_len;
        if (start < 6 && start < cap) {
            for (size_t i = start; i < 6 && i < cap; ++i)
                cache[i] = init[i];
        }
        cur_len = cur_len < 6 ? 6 : cur_len;
    }

    /* compute new terms up to needed size */
    for (size_t i = cur_len; i < need; ++i) {
        seq_t val = 0;
        val += 11 * cache[i - 1];
        val -= 9  * cache[i - 2];
        val -= 15 * cache[i - 3];
        val += 20 * cache[i - 4];
        val -= 6  * cache[i - 5];
        cache[i] = val;
    }
    cur_len = need;

    return cache[(size_t)n - 1];
}
