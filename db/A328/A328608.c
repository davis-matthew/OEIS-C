/*
 * A328608  Numbers whose binary indices have no part circularly followed by a
 *          divisor or a multiple.
 * https://oeis.org/A328608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ok_binary_indices
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A328608 – numbers whose binary indices have no part circularly
   followed by a divisor or a multiple.  The function finds n-th such
   number by incremental search and memoization. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ok_binary_indices.h"

static seq_t *cache = NULL;           /* memoised sequence values   */
static size_t cache_cap = 0;          /* current allocation size    */
static size_t cache_len = 0;          /* number of values stored    */
static idx_t  last_candidate = 0;     /* last tested integer (idx_t) */

/* Ensure capacity for at least 'need' elements in the cache. */
static void ensure_cache_space(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* Allocation failed – abort. */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* -------------------------------------------------------------
   Public function required by the build system.
   ------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;       /* undefined for n<1 – return 0 for safety */

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Generate terms until we have at least n elements. */
    for (idx_t m = last_candidate + 1; cache_len < n; ++m) {
        last_candidate = m;
        if (ok_binary_indices(m)) {
            ensure_cache_space(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
