/*
 * A380635  a(1) = 1; a(n+1) = Sum_{d^2|n} a(n/d^2).
 * https://oeis.org/A380635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = Σ_{d²|n} a(n/d²) */
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

static seq_t *cache = NULL;
static size_t  cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size > (size_t)n) return;

    if (!cache) {
        cache = alloc_seq_array((size_t)n + 1);
        cache_size = (size_t)n + 1;
        mem_set(cache, 0, cache_size * sizeof(seq_t));
    } else {
        size_t old_size = cache_size;
        grow_seq_array(&cache, &cache_size, (size_t)n + 1);
        for (size_t i = old_size; i < cache_size; ++i)
            cache[i] = 0;
    }
    cache[0] = 0;
    cache[1] = 1;
}

static void ensure_computed(idx_t n)
{
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i] != 0) continue;
        idx_t N   = i - 1;
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= N; ++d) {
            if (N % (d * d) == 0) {
                idx_t q = N / (d * d);
                sum += cache[q];
            }
        }
        cache[i] = sum;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    ensure_computed(n);
    return cache[n];
}
