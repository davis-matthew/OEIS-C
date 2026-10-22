/*
 * A356989  a(n) = n - a^[3](n - a^[4](n-1)) with a(1) = 1, where a^[3](n) =
 *          a(a(a(n))) and a^[4](n) = a(a(a(a(n)))).
 * https://oeis.org/A356989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - a^[3](n - a^[4](n-1)) * Recurrence: a(1) = 1
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_capacity(idx_t n) {
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static inline seq_t get_a(idx_t k) { return cache[k-1]; }

seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n<1 */

    if (n > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
                continue;
            }
            /* a^[4](i-1) */
            seq_t v1 = get_a(i-1);
            seq_t v2 = get_a((idx_t)v1);
            seq_t v3 = get_a((idx_t)v2);
            seq_t v4 = get_a((idx_t)v3);

            /* n - a^[4](n-1) */
            idx_t t = i - (idx_t)v4;

            /* a^[3](t) */
            seq_t w1 = get_a(t);
            seq_t w2 = get_a((idx_t)w1);
            seq_t w3 = get_a((idx_t)w2);

            cache[i-1] = ((seq_t)i) - w3;
        }
        cache_len = n;
    }
    return cache[n-1];
}
