/*
 * A295024  Cubes whose largest digit is 9.
 * https://oeis.org/A295024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 13, u32 n <= 870, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: cubes whose largest digit is 9
   Formula: a(n) = A294999(n)^3
   where A294999(n) is the nth integer whose cube contains a 9
   This implementation generates cubes in increasing order and
   selects those whose decimal representation contains a 9. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/has9.h"

/* Cache for computed terms */
static seq_t *cache = NULL;
static size_t cap = 0, size = 0;
static idx_t next_root = 1;   /* next integer to test for a cube */

/* Grow cache capacity if needed */
static void ensure_capacity(size_t needed) {
    if (cap >= needed) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failure handled by caller as needed */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;
    cap = new_cap;
}

/* Main function to compute a(n) */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;   /* offset is 1 */

    /* Generate terms until we have at least n terms */
    while (size < n) {
        ensure_capacity(size + 1);

        seq_t m = (seq_t)next_root;
        seq_t cube = m * m * m;   /* compute cube */

        if (has9(cube)) {
            cache[size++] = cube;   /* store valid cube */
        }
        next_root++;   /* move to next integer */
    }
    return cache[n - 1];
}
