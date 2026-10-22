/*
 * A301788  Number of nX6 0..1 arrays with every element equal to 0 or 1
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - a(n-4) - a(n-5) - a(n-6) Initial
 *            terms: a(1)=13, a(2)=22, a(3)=38, a(4)=66, a(5)=116, a(6)=205
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 73, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <algorithm>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t max_comp = 0;

    if (n < 1) return (seq_t)0;

    /* Ensure storage up to index n */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];
        if (cache) {
            std::copy(cache, cache + cap, new_cache);
            delete[] cache;
        }
#else
        new_cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
        cache = new_cache;
        cap = new_cap;
    }

    /* Set the known initial values (up to 6) */
    if (max_comp < 6) {
        static seq_t init[7] = {0,13,22,38,66,116,205};
        size_t up_to = n < 6 ? n : 6;
        for (idx_t i = max_comp + 1; i <= up_to; ++i) {
            cache[i] = init[i];
        }
        if (up_to > max_comp) max_comp = up_to;
    }

    /* Compute remaining terms using the recurrence */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t val = cache[i-1];
        val += (seq_t)2 * cache[i-2];
        val -= cache[i-4];
        val -= cache[i-5];
        val -= cache[i-6];
        cache[i] = val;
    }
    max_comp = n;

    return cache[n];
}
