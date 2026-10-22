/*
 * A377945  Numbers k such that the trajectory of k under the `3x-1' map
 *          reaches k+1.
 * https://oeis.org/A377945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   itos, reaches_next, step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the 3x‑1 map reaches k+1 (computed by brute‑force search). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/itos.h"
#include "../utils/reaches_next.h"
#include "../utils/step.h"

/* Simple dynamic array of seq_t values.  */
static seq_t *cache     = NULL;
static size_t cache_sz  = 0;
static size_t cache_cap = 0;
static idx_t  next_test = 1;        /* next integer to test for membership */

/* Grow the cache to accommodate more elements.  */
static void grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a new term to the cache.  */
static void append(seq_t v) {
    if (cache_sz == cache_cap) grow_cache();
    cache[cache_sz++] = v;
}

/* Return the n-th element of the sequence (a(1) is the first term).  */
seq_t a(idx_t n) {
    if (n < 1) abort(); /* sequence is defined for n >= 1 only */

    while (cache_sz < (size_t)n) {
        /* Find the next integer that satisfies the property. */
        while (!reaches_next(itos(next_test))) ++next_test;
        append(itos(next_test));
        ++next_test;
    }
    return cache[(size_t)n - 1];
}
