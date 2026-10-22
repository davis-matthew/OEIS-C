/*
 * A327376  BII-numbers of set-systems with vertex-connectivity 3.
 * https://oeis.org/A327376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_vertex_connectivity_3, indices_of, is_connected_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the BII‑numbers of set‑systems with vertex‑connectivity 3
   by brute‑forcing every integer and testing its derived set‑system.
   a(1) is the first such integer, a(2) the second, etc. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_vertex_connectivity_3.h"
#include "../utils/indices_of.h"
#include "../utils/is_connected_mask.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Cache of results for fast O(1) lookup after initial computation. */
static seq_t *cache        = NULL;
static idx_t  cache_len    = 0;
static idx_t  cache_cap    = 0;
static idx_t  next_candidate = 1;  /* next integer to test */

static void ensure_cache(idx_t need)
{
    if (cache_len >= need)
        return;

    if (cache_cap < need) {
        cache_cap = (cache_cap > 0) ? cache_cap * 2 : 128;
        while (cache_cap < need)
            cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    while (cache_len < need) {
        if (has_vertex_connectivity_3(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)         /* Undefined below offset; return 0 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
