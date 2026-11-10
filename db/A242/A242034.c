/*
 * A242034  a(n) = lpf(A243937(n)-3), where lpf = least prime factor
 *          (A020639).
 * https://oeis.org/A242034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf(m-3), where m = A243937(n) is the n-th even m >= 6
 *            with lpf(m-1) > lpf(m-3); computed by a linear search with a
 *            cache
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 8747 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t A242034_lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 6;

    while (cache_len < n) {
        idx_t m = next_candidate;
        next_candidate += 2;
        idx_t p3 = A242034_lpf(m - 3);
        if (A242034_lpf(m - 1) > p3) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p3;
        }
    }
    return cache[n - 1];
}
