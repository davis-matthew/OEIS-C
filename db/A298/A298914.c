/*
 * A298914  Number of n X 5 0..1 arrays with every element equal to 0, 3, 5, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-5)-a(n-6)-a(n-7)-a(n-8) Initial
 *            values: a(1)=1,a(2)=3,a(3)=3,a(4)=4,a(5)=7,a(6)=9,
 *            a(7)=15,a(8)=26,a(9)=46
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 39, u64 n <= 75, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t initial[10] = {
    0ULL, // unused 0 index
    1ULL, // a(1)
    3ULL, // a(2)
    3ULL, // a(3)
    4ULL, // a(4)
    7ULL, // a(5)
    9ULL, // a(6)
    15ULL, // a(7)
    26ULL, // a(8)
    46ULL  // a(9)
};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for offset 1, return 0 */

    /* Cached values: cache[1] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* highest index currently stored */

    /* Initialize cache with base values if empty */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (!cache) abort();
        for (idx_t i = 1; i <= 9; ++i) {
            cache[i] = initial[i];
        }
    }

    /* Return from cache if already computed */
    if (n <= cache_len) {
        return cache[n];
    }

    /* Expand cache to fit new n */
    idx_t new_size = n + 1;                    /* index n included */
    if (new_size < cache_len * 2) new_size = cache_len * 2;
    if (new_size < 10) new_size = 10;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + cache[i - 2];
        val += cache[i - 3];
        val += cache[i - 5];
        val -= cache[i - 6];
        val -= cache[i - 7];
        val -= cache[i - 8];
        cache[i] = val;
    }

    cache_len = n;
    return cache[n];
}
