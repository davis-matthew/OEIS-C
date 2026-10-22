/*
 * A361747  Lexicographically earliest sequence of distinct positive integers
 *          such that a(n) and a(n-1) share at least one identical trit at the
 *          same position in their balanced ternary representations.
 * https://oeis.org/A361747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the lexicographically earliest sequence of distinct
 *            positive integers such that a(n) and a(n-1) share at least one
 *            identical trit at the same position in their balanced ternary
 *            representations
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   compute_digits, has_shared_trit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/compute_digits.h"
#include "../utils/has_shared_trit.h"

/* ensure unsigned index array can hold up to max_idx */
static void ensure_used_capacity(uint64_t max_idx, uint16_t **used, size_t *used_cap)
{
    if (max_idx < *used_cap) return;
    size_t new_cap = (*used_cap == 0) ? 8 : *used_cap * 2;
    while (new_cap <= max_idx) new_cap <<= 1;
    uint16_t *new_arr = (uint16_t *)realloc(*used, new_cap * sizeof(uint16_t));
    if (new_arr == NULL) abort();
    memset(new_arr + *used_cap, 0, (new_cap - *used_cap) * sizeof(uint16_t));
    *used = new_arr;
    *used_cap = new_cap;
}

/* ensure cache can hold up to n */
static void ensure_cache_capacity(size_t n, seq_t **cache, size_t *cache_cap)
{
    if (n < *cache_cap) return;
    size_t new_cap = (*cache_cap == 0) ? 8 : *cache_cap * 2;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (new_arr == NULL) abort();
    *cache = new_arr;
    *cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;            /* last computed index */
    static uint16_t *used     = NULL;        /* 0 = not used, 1 = used */
    static size_t used_cap    = 0;

    if (n == 0) return (seq_t)0;            /* offset unused */
    if ((size_t)n <= cache_len) return cache[(size_t)n];

    /* initialization for the base term */
    if (cache_len == 0) {
        ensure_cache_capacity(1, &cache, &cache_cap);
        cache[1] = (seq_t)1;
        ensure_used_capacity(1, &used, &used_cap);
        used[1] = 1;
        cache_len = 1;
    }

    signed char prev_digits[128];
    size_t prev_len = compute_digits(cache[cache_len], prev_digits);

    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t cand = (seq_t)1;
        while (1) {
            uint64_t cval = (uint64_t)cand;          /* cand is small enough */
            ensure_used_capacity(cval, &used, &used_cap);
            if (used[cval]) { cand++; continue; }

            if (has_shared_trit(cache[i-1], cand, prev_digits, prev_len)) {
                ensure_cache_capacity(i, &cache, &cache_cap);
                cache[i] = cand;
                used[cval] = 1;
                break;
            }
            cand++;
        }
        /* update previous digits for next iteration */
        prev_len = compute_digits(cache[i], prev_digits);
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
