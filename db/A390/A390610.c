/*
 * A390610  Irregular triangle read by rows: T(n,k) is the sum of composite
 *          numbers in the k-th 2-dense sublist of divisors of n, with n >= 1,
 *          k >= 1.
 * https://oeis.org/A390610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = sum of composite divisors of n in the k-th maximal
 *            2-dense run of divisors (consecutive divisors with ratio <= 2);
 *            a(n) = row-major flattening, built by direct divisor enumeration
 * Range:     u8 n <= 234, u16 n <= 10607, u32 n <= 10607, u64 n <= 10607,
 *            u128 n <= 10607, big any n
 * Validated: standard, pass, 111 terms through n = 9312 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A390610_is_composite(idx_t d) {
    if (d < 4) return false;
    if (d % 2 == 0) return true;
    for (idx_t p = 3; p * p <= d; p += 2)
        if (d % p == 0) return true;
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_row = 1;

    if (n == 0) return 0;
    while ((idx_t)cache_len < n) {
        idx_t r = next_row++;
        seq_t seg = 0;
        idx_t prev = 0;
        for (idx_t d = 1; d <= r; ++d) {
            if (r % d != 0) continue;
            if (prev != 0 && d > prev * 2) {
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 64;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = seg;
                seg = 0;
            }
            if (A390610_is_composite(d)) seg += (seq_t)d;
            prev = d;
        }
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 64;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = seg;
    }
    return cache[(size_t)(n - 1)];
}
