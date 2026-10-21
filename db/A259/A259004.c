/*
 * A259004  Number of (n+2)X(6+2) 0..1 arrays with every 3X3 subblock sum of
 *          the two medians of the central row and column plus the two sums of
 *          the diagonal and antidiagonal nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A259004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1..24) given; a(n) = a(n-1) + a(n-12) - a(n-13) for n>24 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

static const seq_t initial[24] = {
    330684, 317141, 915452, 780329, 321144, 292814, 49717, 42876,
    35111, 36146, 36695, 38298, 39684, 36974, 37838, 36576,
    39450, 40036, 39480, 40734, 38626, 39770, 40339, 41946
};

#if defined(__cplusplus)
static std::vector<seq_t> cache;
#else
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void ensure_capa(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n) {
    if (n <= 24) return initial[n-1];

#if defined(__cplusplus)
    if (cache.empty()) {
        cache.assign(initial, initial + 24);
    }
    idx_t cur = (idx_t)cache.size();
#else
    if (cache_len == 0) {
        ensure_capa(24);
        for (size_t i = 0; i < 24; ++i) cache[i] = initial[i];
        cache_len = 24;
        cache_cap = (cache_cap > 24 ? cache_cap : 24);
    }
    idx_t cur = (idx_t)cache_len;
#endif

    while (cur < n) {
        /* a(cur+1) = a(cur) + a(cur-11) - a(cur-12)   (1‑based indices) */
        seq_t val = cache[cur-1] + cache[cur-12] - cache[cur-13];
#if defined(__cplusplus)
        cache.push_back(val);
#else
        if (cur >= cache_cap) ensure_capa(cur + 1);
        cache[cur] = val;
        cache_len++;
#endif
        cur++;
    }
    return cache[cur-1];
}
