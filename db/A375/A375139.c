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
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Return true iff k satisfies the condition for the sequence. */
static bool
k_satisfies(idx_t k)
{
    idx_t   i         = 0;
    idx_t   prev_pos  = 0;
    seq_t   parts[64];   /* number of 1‑bits in k <= 64 for 64‑bit k */
    size_t  len       = 0;
    idx_t   tmp       = k;

    /* Build the part lengths in order of increasing positions. */
    while (tmp) {
        if (tmp & 1U) {
            idx_t pos  = i + 1U;
            parts[len++] = (seq_t)(pos - prev_pos);
            prev_pos = pos;
        }
        tmp >>= 1U;
        i++;
    }

    /* Compute leaders of strictly increasing runs in the reversed order. */
    seq_t   leaders[64];
    size_t  lcnt = 0;
    seq_t   prev_part = 0;

    for (size_t j = 0; j < len; ++j) {
        seq_t part = parts[len - 1 - j];       /* reversed */
        if (lcnt == 0) {
            leaders[lcnt++] = part;
            prev_part = part;
        } else {
            if (part > prev_part) {
                /* Continuation of current run */
                prev_part = part;
            } else {
                /* New run starts */
                leaders[lcnt++] = part;
                prev_part = part;
            }
        }
    }

    /* Check if leaders are NOT weakly decreasing, i.e., some increase. */
    for (size_t p = 0; p + 1 < lcnt; ++p) {
        if (leaders[p] < leaders[p + 1])
            return true;
    }
    return false;
}

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
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
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
