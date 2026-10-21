/*
 * A257409  Values of n such that there are exactly 2 solutions to x^2 - y^2 =
 *          n, with x > y >= 0.
 * https://oeis.org/A257409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A257409: a(n) is the nth positive integer n having exactly two solutions
   to x^2 - y^2 = n with x > y >= 0.  The code searches for such n in increasing
   order and memoises the results. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ensure the cache contains at least n entries */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    if (n == 0) return 0; /* undefined for n=0, but return 0 safely */

    if (cache_len < n) {
        /* grow cache if needed */
        if (cache_cap < n) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 64;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* out of memory */
            cache = tmp; cache_cap = new_cap;
        }
        /* generate more terms */
        idx_t cur = cache_len ? (idx_t)cache[cache_len - 1] + 1 : 1;
        while (cache_len < n) {
            /* count unordered factor pairs with same parity */
            int cnt = 0;
            for (idx_t d = 1; d * d <= cur; ++d) {
                if (cur % d == 0) {
                    idx_t e = cur / d;
                    if ( (d & 1) == (e & 1) ) {
                        ++cnt;
                        if (cnt > 2) break;
                    }
                }
            }
            if (cnt == 2) {
                cache[cache_len++] = (seq_t)cur;
            }
            ++cur;
        }
    }
    return cache[n - 1];
}
