/*
 * A258712  Motzkin numbers A001006 read mod 5.
 * https://oeis.org/A258712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Motzkin numbers A001006 modulo 5
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 89 terms through n = 88 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 0;
    const seq_t MOD = (seq_t)5;

    /* Lazy initialization */
    if (cache_len == 0) {
        cache_len = 2;
        cache = alloc_seq_array(cache_len);
        cache[0] = (seq_t)1 % MOD;
        cache[1] = (seq_t)1 % MOD;
        computed_up_to = 1;
    }

    /* Grow cache if needed */
    if (n + 1 > cache_len) {
        grow_seq_array(&cache, &cache_len, n + 1);
    }

    /* Compute missing values up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        if (i >= 2) {
            for (idx_t k = 0; k <= i - 2; ++k) {
                sum = (sum + cache[k] * cache[i - 2 - k]) % MOD;
            }
        }
        cache[i] = (i == 0 ? cache[0] : (cache[i - 1] + sum) % MOD);
    }
    if (computed_up_to < n) {
        computed_up_to = n;
    }

    return cache[n];
}
