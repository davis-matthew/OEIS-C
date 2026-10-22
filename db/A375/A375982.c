/*
 * A375982  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 2, 3, or 5.
 * https://oeis.org/A375982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-4)-a(n-5)+a(n-6)+a(n-7)-a(n-11) Initial
 *            values: a(0)=1,a(1)=2,a(2)=4,a(3)=6,a(4)=8,a(5)=11,a(6)=14,
 *            a(7)=18,a(8)=25,a(9)=37,a(10)=53
 * Range:     u8 n <= 14, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base values for 0 ≤ n ≤ 10 */
    static const seq_t base[11] = {
        1, 2, 4, 6, 8, 11, 14, 18, 25, 37, 53
    };

    /* dynamic cache */
    static seq_t *cache = NULL;
    static idx_t cache_last = 0;  /* last index stored in cache */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure – undefined */
        for (idx_t i = 0; i < 11; ++i) cache[i] = base[i];
        cache_last = 10;
    }

    /* if requested term already cached, return it */
    if (n <= cache_last) return cache[(size_t)n];

    /* enlarge cache to hold up to index n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0; /* allocation failure – undefined */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        seq_t sum_pos = cache[i-1] + cache[i-4] + cache[i-6] + cache[i-7];
        seq_t sum_neg = cache[i-5] + cache[i-11];
        cache[i] = sum_pos - sum_neg;
    }

    cache_last = n;
    return cache[(size_t)n];
}
