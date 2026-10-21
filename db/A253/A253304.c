/*
 * A253304  Numbers n such that the sum of the heptagonal numbers H(n) and
 *          H(n+1) is equal to the octagonal number O(m) for some m.
 * https://oeis.org/A253304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+62*a(n-2)-62*a(n-3)-a(n-4)+a(n-5); Initial values:
 *            a(1)=1, a(2)=22, a(3)=77, a(4)=1376, a(5)=4785
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;  /* array of calculated terms, index 0 unused   */
    static idx_t cache_cap  = 0;      /* allocated length (>= cache_len + 1)          */
    static idx_t cache_len  = 0;      /* highest index with a defined value           */

    if (n < 1) {
        return (seq_t)0;             /* undefined for n < 1                          */
    }

    /* Ensure that the first five terms are stored. */
    if (cache_len < 5) {
        /* Need space for indices 0 … 5 (index 0 unused). */
        idx_t need = 6;
        if (cache_cap < need) {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
            cache_cap = need;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)77;
        cache[4] = (seq_t)1376;
        cache[5] = (seq_t)4785;
        cache_len = 5;
    }

    if (n <= cache_len) {
        return cache[n];
    }

    /* Ensure capacity for the requested index. */
    if (cache_cap <= n) {
        idx_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        seq_t prev1 = cache[k - 1];
        seq_t prev2 = cache[k - 2];
        seq_t prev3 = cache[k - 3];
        seq_t prev4 = cache[k - 4];
        seq_t prev5 = cache[k - 5];

        seq_t tmp1 = prev1 + 62 * prev2 + prev5;
        seq_t tmp2 = 62 * prev3 + prev4;
        cache[k] = tmp1 - tmp2;
    }

    cache_len = n;
    return cache[n];
}
