/*
 * A290032  a(0)=0; for n > 0, a(n) is the sum of the partial sums of binary
 *          digits of a(n-1) + 1, starting from the least significant digit,
 *          and given weights 1, 2, 2^2, etc.
 * https://oeis.org/A290032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(0) = 0 For n > 0, let y = a(n-1) + 1. If k = floor(log2(y)) +
 *            1 and p = popcount(y), then a(n) = p * 2^k – y. This follows
 *            from the definition that a(n) is the sum of the weighted partial
 *            sums of the binary digits of y
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation                 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of valid entries in cache */

    /* Allocate cache for all terms up to n, computing new ones            */
    if (n >= cache_size) {
        /* grow the cache to hold indices 0 … n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* memory allocation failed */
        cache = tmp;

        /* initialise a(0) if cache is empty          */
        if (cache_size == 0) {
            cache[0] = 0;
            cache_size = 1;
        }

        /* compute terms from cache_size up to n      */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t y    = prev + 1;          /* y = a(i‑1) + 1 */

            /* popcount(y) and bit‑length of y      */
            seq_t  pop  = 0;
            idx_t  k    = 0;
            seq_t  temp = y;
            while (temp > 0) {
                if (temp & 1) ++pop;
                temp >>= 1;
                ++k;
            }

            seq_t pow2 = (seq_t)1 << k;     /* 2^k */
            seq_t val  = pop * pow2 - y;    /* a(i)  */

            cache[i] = val;
        }

        cache_size = n + 1;
    }

    return cache[n];
}
