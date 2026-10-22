/*
 * A370920  Positive integers in whose ternary representation 2 occurs at
 *          least once, and every 2 is followed by 0.
 * https://oeis.org/A370920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose ternary representation *
 *            contains at least one digit 2, and every 2 is immediately
 *            followed by a 0. * The sequence is enumerated in increasing
 *            order (a(1)=6)
 * Range:     u8 n <= 39, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   valid_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/valid_seq_t.h"

/* static cache of generated terms */
static seq_t *cache = NULL;       /* pointer to the cache array */
static size_t cache_size = 0;     /* number of elements stored */
static size_t cache_cap   = 0;     /* current capacity */
static idx_t   last_checked = 0;  /* last integer examined (uint64_t) */

/* append a value to the static cache, growing if necessary */
static void cache_append(seq_t val) {
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) exit(EXIT_FAILURE);   /* allocation failed */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1; undefined for n==0 */

    /* generate terms until we reach the requested index */
    while (cache_size < n) {
        /* search for the next valid integer */
        do {
            last_checked++;             /* move to the next candidate */
        } while (!valid_seq_t((seq_t)last_checked));

        /* store the found term */
        cache_append((seq_t)last_checked);
    }

    return cache[n - 1];
}
