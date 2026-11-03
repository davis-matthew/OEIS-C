/*
 * A386746  a(n) = n^3*sigma_2(n).
 * https://oeis.org/A386746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * sigma_2(n) where sigma_2(n) = sum_{d|n} d^2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 79, u64 n <= 6551,
 *            u128 n <= 9000, big any n
 * Validated: standard, pass, 49 terms through n = 8992 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;
static size_t cap = 0;

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    /* Ensure cache has space for index n */
    ensure_cache(n, &cache, &cap);
    if (cache[n] != 0) /* already computed */
        return cache[n];

    seq_t sigma2 = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            sigma2 += d1 * d1;
            idx_t d2 = n / i;
            if (d2 != i) {
                seq_t d2s = (seq_t)d2;
                sigma2 += d2s * d2s;
            }
        }
    }
    seq_t n_seq = (seq_t)n;
    seq_t n_cub = n_seq * n_seq * n_seq;
    seq_t ans = n_cub * sigma2;
    cache[n] = ans;
    return ans;
}
