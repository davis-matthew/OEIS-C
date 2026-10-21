/*
 * A292454  Numbers where 4 outnumbers any other digit.
 * https://oeis.org/A292454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th positive integer x such that count_digit(x,4) >
 *            max_{d≠4} count_digit(x,d)
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   alloc_cache, digit_counts, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

static seq_t *cache      = NULL;     /* cached terms */
static size_t cache_cap  = 0;        /* array capacity   */
static idx_t   cache_len = 0;        /* terms stored     */
static idx_t   next_candidate = 4;   /* next integer to test */

/* enlarge the cache so it can hold at least min_cap entries */
static void ensure_cache_cap(size_t min_cap) {
    if (cache_cap >= min_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;

    seq_t *new_buf = alloc_cache((idx_t)new_cap);
    for (idx_t i = 0; i < cache_len; ++i) {
        new_buf[i] = cache[i];
    }
    if (cache) free_cache(cache);

    cache     = new_buf;
    cache_cap = new_cap;
}

/* true iff x has a count of digit 4 strictly larger than every other digit */
static bool has_more_4s(seq_t x) {
    unsigned char cnt[10];
    digit_counts(x, cnt);
    unsigned char c4 = cnt[4];
    for (int d = 0; d < 10; ++d) {
        if (d != 4 && cnt[d] >= c4) return false;
    }
    return true;
}

/* ensure at least n terms have been generated and stored in cache */
static void ensure_terms_to(idx_t n) {
    if (cache_len >= n) return;
    ensure_cache_cap((size_t)n);
    while (cache_len < n) {
        seq_t val = (seq_t)next_candidate;
        if (has_more_4s(val)) {
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n) {
    if (!n) return (seq_t)0;          /* undefined for n<1, harmless */
    ensure_terms_to(n);
    return cache[n - 1];
}
