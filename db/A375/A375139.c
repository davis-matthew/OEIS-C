/*
 * A375139  Numbers k such that the leaders of strictly increasing runs in the
 *          k-th composition in standard order are not weakly decreasing.
 * https://oeis.org/A375139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   k_satisfies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = k such that the leaders of strictly increasing runs
 *              in the k‑th composition in standard order are not weakly
 *              decreasing.  Computed by brute‑force scan of compositions.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/k_satisfies.h"

/* Cache for already computed terms */
static seq_t *cache = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static idx_t    next_k   = 1;        /* next candidate k to test */

/* Ensure the cache has capacity for at least 'needed' elements. */
static void
ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* behaviour below offset is irrelevant */

    /* Generate terms until we reach index n. */
    while (cache_len < (size_t)n) {
        if (k_satisfies(next_k)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
        }
        next_k++;
    }
    return cache[n - 1];
}
