/*
 * A253654  Indices of pentagonal numbers (A000326) which are also centered
 *          pentagonal numbers (A005891).
 * https://oeis.org/A253654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 9*a(n-2) + a(n-3); Initial values: a(1)=1,
 *            a(2)=6, a(3)=46
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* array of computed terms, 1‑based */
static idx_t  computed = 0;           /* largest index that has been computed */
static size_t alloc_sz = 0;           /* current size of the allocated array */

/* Ensure that cache has terms up to index n. */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        /* Allocate at least space for the first three terms. */
        alloc_sz = (n < 4 ? 4 : n) + 1;
        cache = (seq_t *)calloc(alloc_sz, sizeof(seq_t));
        if (cache == NULL) return; /* memory failure – caller will get 0 */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)46;
        computed = 3;
    }
    if (n <= computed) return;

    /* Grow the array if necessary. */
    if (n + 1 > alloc_sz) {
        size_t new_sz = alloc_sz * 2;
        if (new_sz < n + 1) new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) return; /* memory failure */
        cache = tmp;
        alloc_sz = new_sz;
    }

    /* Compute the new terms using the recurrence. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)(
                    (seq_t)9 * cache[i-1]
                  - (seq_t)9 * cache[i-2]
                  + cache[i-3] );
    }
    computed = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
