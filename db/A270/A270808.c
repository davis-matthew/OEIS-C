/*
 * A270808  First differences of A270807, divided by 2.
 * https://oeis.org/A270808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A270807(n+2) - A270807(n+1))/2, where A270807(1) = 1,
 *            A270807(k+1) = A270807(k) + A270807(k)/gpf(A270807(k)) + 1
 * Range:     u8 n <= 42, u16 n <= 1069, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9177 (2026-11-07)
 * Helpers:   largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/mem.h"

static seq_t A270807_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    if (n <= cache_len && cache != NULL)
        return cache[n];

    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) return 0;
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 1;
    }

    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev   = cache[i - 1];
        uint64_t pu  = (uint64_t)prev;
        uint64_t pf  = largest_prime_factor(pu);
        seq_t inc    = prev / (seq_t)pf;
        cache[i]     = prev + inc + 1;
    }

    cache_len = n;
    return cache[n];
}

seq_t a(idx_t n)
{
    seq_t hi = A270807_a(n + 2);
    seq_t lo = A270807_a(n + 1);
    return (hi - lo) / 2;
}
