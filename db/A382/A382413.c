/*
 * A382413  Numbers with at least one zero in their base-7 representation.
 * https://oeis.org/A382413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_zero_base7_idx, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th non‑negative integer that contains a zero
   digit in its base‑7 representation (offset 1). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has_zero_base7_idx.h"

static seq_t *cache = NULL;           /* stored terms          */
static idx_t cache_len  = 0;          /* number of terms stored */
static idx_t cache_cap  = 0;          /* allocated capacity    */
static idx_t next_candidate = 0;      /* next integer to test */

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n)
        return;

    /* Resize allocation if needed. */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *new_cache = seq_realloc(cache,
                                      (size_t)cache_cap,
                                      (size_t)new_cap);
        if (!new_cache)
            abort();                 /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until the cache has n entries. */
    for (; cache_len < n; ++next_candidate) {
        if (has_zero_base7_idx(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
    }
}

/* OEIS entry function. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined for offset 1, return 0 as safety */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
