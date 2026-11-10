/*
 * A304685  a(n) = A000699(n) (mod 3).
 * https://oeis.org/A304685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000699(n) mod 3, with A000699(n) = (n-1)*Sum_{i=1..n-1}
 *            A000699(i)*A000699(n-i), A000699(0) = A000699(1) = 1; computed
 *            with all values reduced mod 3
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 101 terms through n = 1678 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n == 0 || n == 1) return 1;

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *new_cache = mem_new(seq_t, new_cap);
            mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }

        idx_t k = cache_len;
        seq_t sum = 0;
        for (idx_t i = 1; i < k; i++) {
            sum = (sum + cache[i] * cache[k - i]) % 3;
        }
        seq_t f = (seq_t)((k - 1) % 3);
        cache[k] = (f * sum) % 3;
        cache_len++;
    }

    return cache[n];
}
