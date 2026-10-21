/*
 * A270079  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 62", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 12, a(3) = 20, a(4) = 32, a(5) = 44,
 *            a(6) = 68, a(7) = 72, a(8) = 100, a(9) = 112, a(10) = 156, a(11)
 *            = 160, a(12) = 204. For n ≥ 13: a(n) = a(n−1) + a(n−2) − a(n−3)
 *            + a(n−4) − a(n−5) − a(n−6) + a(n−7)
 * Range:     u8 n <= 13, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[13] = {
    1, 5, 12, 20, 32, 44, 68, 72, 100, 112, 156, 160, 204
};

seq_t a(idx_t n)
{
    /* Cached values to allow arbitrary order calls. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* size of the cache, number of stored terms */

    /* Initialize cache with the first 13 known terms. */
    if (cache_len == 0)
    {
        cache_len = 13;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return 0;  /* malloc failed; caller can handle 0 as error */
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* If the requested n is already in the cache, return it directly. */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache to include terms up to n. */
    size_t new_len = (size_t)(n + 1);
    seq_t *temp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!temp) return 0;  /* realloc failed */
    cache = temp;
    /* Compute missing terms from the current cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i <= 12)
        {
            cache[i] = init[i];
        }
        else
        {
            /* recurrence: a(n)=a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7)  */
            /* rearranged to avoid underflow: pos has all + terms, neg has all - terms. */
            seq_t pos = cache[i-1] + cache[i-2] + cache[i-4] + cache[i-7];
            seq_t neg = cache[i-3] + cache[i-5] + cache[i-6];
            cache[i] = pos - neg;   /* on unsigned types this is safe because pos ≥ neg for n ≥ 13 */
        }
    }
    cache_len = (idx_t)new_len;
    return cache[n];
}
