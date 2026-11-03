/*
 * A399536  G.f. A(x) satisfies: A(x) = (1 + x + x^3 * A(2*x)) / (1 - x^2).
 * https://oeis.org/A399536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 15, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A399536: a(n) = 1 for n < 3; a(n) = a(n-2) + 2^(n-3) * a(n-3). */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Persistent cache of computed terms. */
static seq_t *cache = NULL;   /* array of a(n) */
static size_t cap   = 0;      /* allocated size */
static idx_t  max_comp = 0;   /* highest index computed */

seq_t a(idx_t n)
{
    /* Initialise first three terms on first call. */
    if (cache == NULL) {
        grow_seq(&cache, &cap, 3);
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        max_comp = 2;
    }

    /* Return already computed value. */
    if (n <= max_comp)
        return cache[n];

    /* Grow the cache to accommodate up to n. */
    grow_seq(&cache, &cap, n + 1);

    /* Compute missing terms. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i < 3) {
            cache[i] = (seq_t)1;
            continue;
        }
        seq_t pow2 = ipow_idx((seq_t)2, i - 3);
        cache[i] = cache[i - 2] + pow2 * cache[i - 3];
    }

    max_comp = n;
    return cache[n];
}
