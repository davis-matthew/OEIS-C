/*
 * A277357  a(1) = 1; for n > 1, a(n) = (2^n-1)*a(n-1) + 1.
 * https://oeis.org/A277357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n-1)*a(n-1) + 1 with a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated array size */
    static idx_t last   = 0;   /* largest n for which cache[n] is valid */

    if (n == 0) return 0; /* undefined, but harmless */

    /* Ensure the cache array can hold indices up to n */
    if (cache == NULL) {
        cap = (n > 1) ? n + 1 : 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * cap);
    } else if ((size_t)n + 1 > cap) {
        /* grow the array to at least n+1 elements */
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {
            /* allocation failure – return 0 as a fail‑safe value */
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute terms up to n if they are not already in cache */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;   /* base case */
            continue;
        }
        seq_t prev = cache[i - 1];
        seq_t two_n = pow2(i);                  /* 2^i */
        seq_t mult  = two_n - (seq_t)1;         /* 2^i - 1 */
        cache[i] = mult * prev + (seq_t)1;
    }
    last = n;

    return cache[n];
}
