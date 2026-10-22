/*
 * A349057  Numbers k such that the k-th composition in standard order is not
 *          weakly alternating.
 * https://oeis.org/A349057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_not_weakly_alternating
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = n-th integer k for which the k‑th composition in standard
 * order (graded reverse‑lexicographic) is NOT weakly alternating.
 * The function returns the n-th such integer, with a(1) = 37. */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_not_weakly_alternating.h"

/* Cache of computed terms */
static seq_t *cache = NULL;          /* array of seq_t values */
static size_t cache_len = 0;         /* number of entries stored */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t processed_k = 0;        /* last k that was checked */

/* Ensure the cache has capacity for at least 'need' entries. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < need) new_cap = need;
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) exit(1);        /* allocation failure */
    if (cache) {
        memcpy(newp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Returned element type: seq_t
 * Index type: idx_t
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1, return 0 */

    /* Fill the cache up to the requested index if necessary */
    while (cache_len < (size_t)n) {
        ++processed_k;
        if (is_not_weakly_alternating(processed_k)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)processed_k;
        }
    }
    return cache[n - 1];
}
