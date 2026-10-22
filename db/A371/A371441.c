/*
 * A371441  a(n) = a(n-1)*3^n + 1 where a(0)=1.
 * https://oeis.org/A371441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; For n > 0: a(n) = a(n-1) * 3^n + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of values stored (next free index) */

    static seq_t *pow_cache = NULL;    /* stores 3^i for i = 0 … n */

    /* Allocate or grow storage as needed */
    if (n + 1 > cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        seq_t *new_pow   = (seq_t *)realloc(pow_cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        pow_cache = new_pow;

        /* First‑time initialization */
        if (cache_size == 0) {
            pow_cache[0] = (seq_t)1;   /* 3^0 */
            cache[0]     = (seq_t)1;   /* a(0) */
            cache_size   = 1;
        }

        /* Compute new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            pow_cache[i] = pow_cache[i - 1] * (seq_t)3;
            cache[i] = cache[i - 1] * pow_cache[i] + (seq_t)1;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
