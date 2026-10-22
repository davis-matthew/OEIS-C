/*
 * A357706  Numbers k such that the k-th composition in standard order has
 *          half-alternating sum and skew-alternating sum both 0.
 * https://oeis.org/A357706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   ok_for_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A357706: numbers k such that the k‑th
   composition in graded reverse‑lexicographic order has both the
   half‑alternating sum and skew‑alternating sum equal to 0. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ok_for_k.h"

/* Cache of found values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t next_k = 0;

/* Ensure the cache holds at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr && new_cap > 0) {
        /* allocation failure – abort is safe for this harness */
        abort();
    }
    cache = new_ptr;
    cache_cap = new_cap;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset is 1, guard against 0 */

    size_t target = (size_t)n;
    while (cache_size < target) {
        if (ok_for_k(next_k)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[target - 1];
}
