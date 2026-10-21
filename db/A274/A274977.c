/*
 * A274977  a(n) = a(n-1) + 3*a(n-2) with n>1, a(0)=1, a(1)=6.
 * https://oeis.org/A274977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 3*a(n-2)
 * Initial values: a(0) = 1, a(1) = 6
 */
#include <stdlib.h>
#include <assert.h>

/* Compute the n-th term of OEIS A274977.  The implementation uses
 * memoisation: terms are computed once and stored in a static array.
 * The array is grown with realloc as required.  Since seq_t may be an
 * arbitrary‑precision type the memory is allocated with malloc /
 * realloc, which is sufficient for the builds used in the oei test
 * harness.  The function is thread‑safe only with respect to the
 * single static cache so it is intended for single‑threaded use. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* number of elements currently stored */

    /* Return cached value if available */
    if (n < (idx_t)cap)
        return cache[(size_t)n];

    /* Grow cache to hold up to index n */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* Reallocation failed – abort to avoid undefined behaviour */
        abort();
    }
    cache = new_cache;

    /* Compute the missing terms */
    for (size_t i = cap; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)6;
        else
            cache[i] = cache[i - 1] + (seq_t)3 * cache[i - 2];
    }

    cap = new_cap;
    return cache[(size_t)n];
}
