/*
 * A333943  Numbers k such that the k-th composition in standard order is a
 *          reversed necklace.
 * https://oeis.org/A333943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k (n ≥ 1) such that the k‑th composition
 *            in standard order is a reversed necklace. The value is obtained
 *            by a search; no closed formula is known
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_min_rotation, is_revnecklace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_min_rotation.h"
#include "../utils/is_revnecklace.h"

/* static cache of found k values */
static seq_t *cache = NULL;      /* array of k values */
static size_t cache_len = 0;     /* number of cached values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t next_k = 1;         /* next k to test */

/* Grow the cache until it contains at least 'need' values. */
static void ensure_cache(size_t need)
{
    while (cache_len < need) {
        if (is_revnecklace(next_k)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)            /* allocation failure – abort */
                    return;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for n=0 */

    if (!cache) {                  /* first call, allocate initial buffer */
        cache = (seq_t*)calloc(16, sizeof(seq_t));
        cache_cap = 16;
    }

    ensure_cache((size_t)n);
    return cache[n - 1];
}
