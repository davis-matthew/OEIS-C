/*
 * A398567  Number of perfect matchings in a 2n X 3 grid graph on the Klein
 *          bottle.
 * https://oeis.org/A398567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4*a(n-1)+16*a(n-2)-6*a(n-3)-16*a(n-4)+4*a(n-5)+a(n-6)
 *            Initial terms: a(1)=16, a(2)=82, a(3)=504, a(4)=3178,
 *            a(5)=20072, a(6)=126872
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;          /* 1‑based cache */
static size_t cache_cap = 0;         /* allocated length (including index 0) */
static size_t cache_size = 0;        /* highest n stored */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but safe */

    if (cache == NULL) {
        /* allocate space for indices 0..7 (index 0 unused) */
        size_t init = 8;
        cache = (seq_t*)malloc((init + 1) * sizeof(seq_t));
        if (!cache) abort();  /* allocation failed */
        cache_cap  = init + 1;
        cache_size = 6;
        cache[1] = 16;
        cache[2] = 82;
        cache[3] = 504;
        cache[4] = 3178;
        cache[5] = 20072;
        cache[6] = 126872;
    }

    if (n <= cache_size) return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    for (size_t i = cache_size + 1; i <= n; ++i) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* positive contributions */
        sum_pos += mul_small(cache[i-1], 4);
        sum_pos += mul_small(cache[i-2], 16);
        sum_pos += mul_small(cache[i-5], 4);
        sum_pos += cache[i-6];

        /* negative contributions */
        sum_neg += mul_small(cache[i-3], 6);
        sum_neg += mul_small(cache[i-4], 16);

        cache[i] = sum_pos - sum_neg;  /* guaranteed positive */
    }
    cache_size = n;
    return cache[n];
}
