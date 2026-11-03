/*
 * A345390  Numbers whose set of divisors contains every digit at least twice.
 * https://oeis.org/A345390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 706, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9367 (2026-11-01)
 * Helpers:   digit_counts, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A345390 – Numbers whose set of divisors contains every digit at least twice.
   This implementation searches successive integers and stores those that satisfy
   the digit–frequency condition in a static cache.  It uses the utilities
   `digit_counts`, `floor_sqrt`, and `ensure_capacity`. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

/* Helper: does integer x have the required property? */
static inline bool has_property(uint64_t x)
{
    int cnt[10] = {0};
    unsigned char digit_cnt[10];
    uint64_t sqrt_u = (uint64_t)floor_sqrt((seq_t)x);

    for (uint64_t d = 1; d <= sqrt_u; ++d) {
        if (x % d == 0) {
            /* divisor d */
            digit_counts((seq_t)d, digit_cnt);
            for (int k = 0; k < 10; ++k) cnt[k] += digit_cnt[k];

            uint64_t other = x / d;
            if (other != d) {
                digit_counts((seq_t)other, digit_cnt);
                for (int k = 0; k < 10; ++k) cnt[k] += digit_cnt[k];
            }
        }
    }
    for (int k = 0; k < 10; ++k)
        if (cnt[k] < 2) return false;
    return true;
}

/* Static cache of discovered terms */
static seq_t *cache_vals = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 0;

/* Append a new term to the cache, resizing if necessary */
static inline void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
        ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache_vals, sizeof(seq_t));
    cache_vals[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, but safe */

    while (cache_len < (size_t)n) {
        ++next_candidate;
        if (has_property(next_candidate)) {
            cache_push((seq_t)next_candidate);
        }
    }
    return cache_vals[n - 1];
}
