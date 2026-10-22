/*
 * A362884  a(n) = (a(n-1)*a(n-2)*a(n-3)+64)/(4*a(n-4)) with a(0) = a(2) =
 *          a(3) = 2 and a(1) = 16.
 * https://oeis.org/A362884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-2)*a(n-3)+64)/(4*a(n-4)) a(0) = 2, a(1) = 16,
 *            a(2) = 2, a(3) = 2. Recurrence holds for n >= 4
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* cached terms */
static idx_t cache_len = 0;     /* number of computed terms */

seq_t a(idx_t n)
{
    /* Ensure base cases in cache */
    if (cache_len == 0) {
        size_t cap = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1); /* abort on allocation failure */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)2;
        cache_len = 4;
    }

    /* Extend cache if needed */
    if (n >= cache_len) {
        size_t cap = cache_len * 2;
        if (cap == 0) cap = 16;
        while (cap <= n) cap <<= 1;   /* double till enough */
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        /* compute new terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t num = cache[i-1] * cache[i-2] * cache[i-3] + (seq_t)64;
            seq_t den = (seq_t)4 * cache[i-4];
            cache[i] = num / den;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
