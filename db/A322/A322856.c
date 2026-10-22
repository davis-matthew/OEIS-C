/*
 * A322856  Number of compositions (ordered partitions) of n into octagonal
 *          pyramidal numbers (A002414).
 * https://oeis.org/A322856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0: a(n) = Σ_{k≥1, pk ≤ n} a(n – pk), where pk
 *            = k·(k+1)·(2k – 1)/2 (octagonal pyramidal numbers)
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;  /* first uncached index */

    if (n >= cache_size) {
        /* Ensure enough capacity for indices up to n */
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cache_cap) {
            size_t cap = new_cap * 2 + 64;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = cap;
        }

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t total = (seq_t)0;
            for (idx_t k = 1; ; ++k) {
                seq_t pk = (seq_t)k * ((seq_t)k + (seq_t)1) *
                           ((seq_t)(2 * k - 1)) / (seq_t)2;
                if (pk > (seq_t)i) break;
                total += cache[i - (idx_t)pk];
            }
            cache[i] = total;
        }

        cache_size = n + 1;
    }
    return cache[n];
}
