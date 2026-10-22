/*
 * A397233  Expansion of 1/(1 - x^4 - x^5 - x^7).
 * https://oeis.org/A397233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5) + a(n-7) a(0) = 1, a(n) = 0 for n < 0
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: number of compositions of n into parts 4, 5, and 7   */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of already computed values   */
static size_t cap    = 0;     /* current cache capacity             */
static size_t stored = 0;     /* number of values stored (== last index + 1) */

static void ensure_capacity(size_t need) {
    if (cap >= need) return;
    size_t new_cap = cap ? cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real program you might handle out of memory here. */
        abort();
    }
    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n) {
    /* cache may be expanded up to index n */
    size_t need = (size_t)n + 1;
    if (stored <= n) {
        ensure_capacity(need);
        for (size_t i = stored; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 1;
            } else {
                seq_t val = 0;
                if (i >= 4) val += cache[i - 4];
                if (i >= 5) val += cache[i - 5];
                if (i >= 7) val += cache[i - 7];
                cache[i] = val;
            }
        }
        stored = need;
    }
    return cache[n];
}
