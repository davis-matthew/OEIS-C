/*
 * A252945  Number of (n+2) X (1+2) 0..2 arrays with every consecutive three
 *          elements in every row, column and nw-se diagonal having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 4*a(n-3) + 2*a(n-4) - 6*a(n-5) +
 *            4*a(n-6) Empirical recurrence (valid for n ≥ 7). Initial values:
 *            a(1)=198, a(2)=747, a(3)=2970, a(4)=11943, a(5)=48024,
 *            a(6)=193059. The function memoises the results so that repeated
 *            calls with arbitrary n are O(1) after the first com...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms stored */
    static idx_t cache_cap = 0;          /* allocated capacity */

    /* initialise cache with the first six terms */
    if (cache_len == 0) {
        cache_cap = 6;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* malloc failed */

        cache[0] = 198;
        cache[1] = 747;
        cache[2] = 2970;
        cache[3] = 11943;
        cache[4] = 48024;
        cache[5] = 193059;
        cache_len = 6;
    }

    if (n <= cache_len)                  /* already computed */
        return cache[n - 1];

    /* grow the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[n - 1];    /* realloc failed – return last known */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t val = 6 * cache[i - 1];
        val -= 9 * cache[i - 2];
        val += 4 * cache[i - 3];
        val += 2 * cache[i - 4];
        val -= 6 * cache[i - 5];
        val += 4 * cache[i - 6];
        cache[i] = val;
    }
    cache_len = n;
    return cache[n - 1];
}
