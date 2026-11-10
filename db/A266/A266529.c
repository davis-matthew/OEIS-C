/*
 * A266529  Terms of A160552 repeated.
 * https://oeis.org/A266529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A160552(floor((n-1)/2)); A160552(0)=0, A160552(1)=1,
 *            A160552(2^i+j) = 2*A160552(j) + A160552(j+1) for 0 <= j < 2^i
 * Range:     u8 n <= 248, u16 n <= 8182, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 106 terms through n = 7824 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t A160552_a(size_t m)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t last = 1;

    if (m + 1 > cap) {
        size_t new_cap = m + 1;
        if (new_cap < 4096) new_cap = 4096;
        seq_t *nc = mem_new(seq_t, new_cap);
        if (cache) {
            for (size_t i = 0; i <= last; ++i) nc[i] = cache[i];
            mem_free(cache);
        }
        cache = nc;
        cap = new_cap;
        cache[0] = 0;
        cache[1] = 1;
    }

    for (size_t k = last + 1; k <= m; ++k) {
        size_t p = 1;
        while ((p << 1) <= k) p <<= 1;
        size_t j = k - p;
        cache[k] = cache[j] * 2 + cache[j + 1];
    }
    if (m > last) last = m;
    return cache[m];
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    return A160552_a((size_t)((n - 1) / 2));
}
