/*
 * A356990  a(n) = n - a^[4](n - a^[5](n-1)) with a(1) = 1, where a^[4](n) =
 *          a(a(a(a(n)))) and a^[5](n) = a(a(a(a(a(n))))).
 * https://oeis.org/A356990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - a^[4](n - a^[5](n-1)) a(1) = 1; other a(n) defined
 *            for n≥2
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Validated: partial, pass, 97 terms through n = 97 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Iterate the sequence 'a' depth times starting from index 'idx'.
   depth ≥ 1 is assumed. */
static inline seq_t iterate_a(const seq_t *a, idx_t idx, unsigned depth) {
    seq_t res = a[idx];
    for (unsigned k = 1; k < depth; ++k) {
        idx = (idx_t)res;
        res = a[idx];
    }
    return res;
}

/* Static cache for memoisation (1‑based indexing). */
static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_max = 0;

/* Allocate or grow the cache to cover at least indices 0…n. */
static void ensure_cache(idx_t n) {
    if (n < cache_capacity) return;
    idx_t newcap = (cache_capacity > 0) ? (cache_capacity << 1) : 8;
    while (newcap <= n) newcap <<= 1;
    cache = mem_resize(seq_t, cache, (newcap + 1));
    cache_capacity = newcap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, safety guard */
    ensure_cache(n);
    if (n <= cache_max) return cache[n];

    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else {
            seq_t inner = iterate_a(cache, i - 1, 5);
            idx_t m = (idx_t)((seq_t)i - inner);
            seq_t outer = iterate_a(cache, m, 4);
            val = (seq_t)i - outer;
        }
        cache[i] = val;
    }
    cache_max = n;
    return cache[n];
}
