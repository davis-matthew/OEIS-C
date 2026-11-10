/*
 * A257244  First differences of A256393: a(n) = A256393(n+1) - A256393(n).
 * https://oeis.org/A257244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A256393(n+1) - A256393(n), where A256393(1) = 2,
 *            A256393(2k) = A256393(2k-1) + gpf(A256393(2k-1)), A256393(2k+1)
 *            = A256393(2k) + lpf(A256393(2k))
 * Range:     u8 n <= 46, u16 n <= 470, u32 n <= 3803, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 109 terms through n = 3873 (2026-11-07)
 * Helpers:   largest_prime_factor, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/mem.h"

static inline idx_t A256393_lpf(idx_t n)
{
    if (n % 2 == 0) return 2;
    for (idx_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return i;
    return n;
}

static seq_t A256393_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0) return (seq_t)0;

    if (cache == NULL) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    if ((size_t)n > cache_len) {
        size_t new_cap = (size_t)n;
        if (new_cap > cache_cap) {
            cache_cap = new_cap;
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
        }
        for (idx_t i = (idx_t)cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[(size_t)i - 2];
            uint64_t f;
            if ((i & 1ULL) == 0) {
                f = largest_prime_factor((uint64_t)prev);
            } else {
                f = A256393_lpf((idx_t)prev);
            }
            cache[(size_t)i - 1] = prev + (seq_t)f;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return A256393_a(n + 1) - A256393_a(n);
}
