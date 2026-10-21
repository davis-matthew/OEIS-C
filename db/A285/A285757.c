/*
 * A285757  a(n) = a(n - a(n - 2)) + a(n - a(n - 8)), with a(i) = i for 1 <= i
 *          <= 9.
 * https://oeis.org/A285757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - a(n-2)) + a(n - a(n-8)), with a(1)=1, a(2)=2, …,
 *            a(9)=9 (offset 1)
 * Range:     u8 n <= 487, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with the first nine values if required */
    if (!cache) {
        cache = (seq_t *)realloc(NULL, 9 * sizeof(seq_t));
        if (!cache) exit(1);
        for (idx_t i = 0; i < 9; ++i)
            cache[(size_t)i] = (seq_t)(i + 1);
        cache_size = 9;
    }

    /* If the cache is somehow smaller than 9, enlarge it */
    if (cache_size < 9) {
        seq_t *tmp = (seq_t *)realloc(cache, 9 * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        for (idx_t i = cache_size; i < 9; ++i)
            cache[(size_t)i] = (seq_t)(i + 1);
        cache_size = 9;
    }

    if (n <= cache_size)          /* value already known */
        return cache[(size_t)(n - 1)];

    /* Extend cache up to n, computing new terms sequentially */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* k >= 10 ensures the following indices are valid */
        seq_t a_k_minus_2 = cache[(size_t)(k - 3)];
        idx_t idx1 = k - (idx_t)a_k_minus_2;

        seq_t a_k_minus_8 = cache[(size_t)(k - 9)];
        idx_t idx2 = k - (idx_t)a_k_minus_8;

        cache[(size_t)(k - 1)] =
            cache[(size_t)(idx1 - 1)] + cache[(size_t)(idx2 - 1)];
    }

    cache_size = n;
    return cache[(size_t)(n - 1)];
}
