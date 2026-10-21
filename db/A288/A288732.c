/*
 * A288732  a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) for n >= 5, where a(0) = 2,
 *          a(1) = 4, a(2) = 6, a(3) = 8, a(4) = 10.
 * https://oeis.org/A288732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) for n >= 5, a(0) = 2, a(1) =
 *            4, a(2) = 6, a(3) = 8, a(4) = 10
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 116, u64 n <= 244,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_length = 0;
    idx_t needed = n + 1;
    idx_t old_len = cache_length;

    if (cache == NULL) {
        idx_t alloc = (needed < 5 ? 5 : needed);
        cache = (seq_t*)malloc(sizeof(seq_t) * alloc);
        if (!cache) abort();           /* memory allocation failed */
        cache_length = alloc;
        /* initialize base values */
        for (idx_t i = 0; i < cache_length; ++i) cache[i] = 0;
        cache[0] = 2; cache[1] = 4; cache[2] = 6; cache[3] = 8; cache[4] = 10;
    } else if (cache_length <= n) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * needed);
        if (!tmp) abort();           /* memory reallocation failed */
        cache = tmp;
        for (idx_t i = cache_length; i < needed; ++i) cache[i] = 0;
        cache_length = needed;
    }

    /* Compute any terms that have not yet been calculated. */
    idx_t start = (old_len > 5 ? old_len : 5);
    if (start <= n) {
        for (idx_t i = start; i <= n; ++i) {
            seq_t diff = cache[i - 4] - cache[i - 5];
            cache[i] = cache[i - 1] + diff * 2;
        }
    }

    return cache[n];
}
