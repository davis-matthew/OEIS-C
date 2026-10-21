/*
 * A263949  Positive integers n such that (n+84)^3 - n^3 is a square.
 * https://oeis.org/A263949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+16*a(n-2)-16*a(n-3)-a(n-4)+a(n-5) (n>5) Initial
 *            values (n=1..5): 28, 476, 1106, 8218, 18256
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is undefined – return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;      /* 1‑based indexing: cache[1] == a(1) */
    static size_t cache_cap = 0;     /* allocated length (elements)      */
    static idx_t  max_computed = 0;  /* highest n already computed       */

    /* Ensure the cache can hold up to index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to the requested index */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        switch ((int)i) {
            case 1: cache[i] = (seq_t)28;       break;
            case 2: cache[i] = (seq_t)476;      break;
            case 3: cache[i] = (seq_t)1106;     break;
            case 4: cache[i] = (seq_t)8218;     break;
            case 5: cache[i] = (seq_t)18256;    break;
            default:
                /* a(i) = a(i-1)+16*a(i-2)-16*a(i-3)-a(i-4)+a(i-5) */
                cache[i] = cache[i-1];
                cache[i] += (seq_t)16 * cache[i-2];
                cache[i] -= (seq_t)16 * cache[i-3];
                cache[i] -= cache[i-4];
                cache[i] += cache[i-5];
                break;
        }
    }

    max_computed = n;
    return cache[n];
}
