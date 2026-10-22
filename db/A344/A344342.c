/*
 * A344342  Numbers k such that k and k + 1 are both Gray-code Niven numbers
 *          (A344341).
 * https://oeis.org/A344342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 2689, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for consecutive Gray‑code Niven numbers */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static inline unsigned int popcount_seq(seq_t x) {
    unsigned int c = 0;
    while (x != 0) {
        x &= x - 1;
        ++c;
    }
    return c;
}

static inline bool is_gcn(seq_t n) {
    seq_t gray = n ^ (n >> 1);
    unsigned int s = popcount_seq(gray);
    return s != 0 && (n % s) == 0;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_checked = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; undefined below */
    while (cache_len < n) {
        ++last_checked;
        seq_t k = (seq_t) last_checked;
        if (is_gcn(k) && is_gcn(k + 1)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                if (new_cap < cache_len + 1) new_cap = cache_len + 1;
                seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
                if (cache && cache_len > 0) {
                    memcpy(new_arr, cache, cache_len * sizeof(seq_t));
                    free(cache);
                }
                cache = new_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
