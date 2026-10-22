/*
 * A301402  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 5 horizontally, vertically or antidiagonally adjacent
 *          elements, with upper left element zero.
 * https://oeis.org/A301402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 7*a(n-2), a(1)=4, a(2)=32
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Fail fast for undefined indices */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;         /* allocated capacity (in seq_t elements)   */
    static size_t max_computed = 0;/* highest index previously computed */

    /* Ensure the cache is large enough for index n. */
    if (cache == NULL) {
        /* Allocate minimally for indices 0..n (index 0 unused). */
        size_t needed = (size_t)n + 1;
        if (needed < 3) needed = 3;           /* ensure space for a(1) and a(2) */
        cache = (seq_t *)malloc(sizeof(seq_t) * needed);
        if (cache == NULL) return (seq_t)0;   /* out‑of‑memory: return 0 */
        memset(cache, 0, sizeof(seq_t) * needed);
        cap = needed;
    } else if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1; /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) return (seq_t)0;          /* out‑of‑memory */
        /* Zero the newly allocated part */
        memset(tmp + cap, 0, sizeof(seq_t) * (new_cap - cap));
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the first two terms if they haven't been set. */
    if (max_computed < 1 && n >= 1) {
        cache[1] = (seq_t)4;
        max_computed = 1;
    }
    if (max_computed < 2 && n >= 2) {
        cache[2] = (seq_t)32;
        max_computed = 2;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = (idx_t)(max_computed + 1); i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)7 + cache[i - 2] * (seq_t)7;
    }
    if (n > max_computed)
        max_computed = (size_t)n;

    return cache[n];
}
