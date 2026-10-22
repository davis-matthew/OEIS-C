/*
 * A348311  a(n) = n! * Sum_{k=1..n} (-1)^k * (k-2) / (k-1)!.
 * https://oeis.org/A348311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n * (a(n‑1) + (-1)^n * (n-2)) for n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored (indices 0..cache_size-1) */

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* realloc may return void*, cast for C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache = tmp;

        /* initialise base case if this is the first allocation */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            cache_size = 1;
        }

        /* compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                /* already set */
                continue;
            }

            /* compute (-1)^i * (i-2) as signed 64‑bit */
            int64_t add = (int64_t)(i - 2);
            if (i & 1) add = -add;

            /* a(i) = i * (a(i-1) + add) */
            seq_t base = cache[i - 1];
            seq_t sum;
            if (add >= 0) {
                sum = base + (seq_t)add;
            } else {
                sum = base - (seq_t)(-add);
            }
            cache[i] = (seq_t)i * sum;
        }

        cache_size = new_size;
    }

    return cache[n];
}
