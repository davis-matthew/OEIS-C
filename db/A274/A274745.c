/*
 * A274745  Number of n X 4 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + a(n-2) - 2*a(n-3) (for n > 4) * Initial
 *            values: a(1)=4, a(2)=22, a(3)=133, a(4)=813
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* cache of already computed terms; indices are 1‑based */
static seq_t *cache = NULL;
static size_t  capacity = 0;   /* allocated size of cache array   */
static idx_t  cached_max = 0;   /* largest n that has been computed */

/* Return the nth term of A274745 (offset 1).  For n < 1 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Handle small n by the known initial values (may overflow seq_t). */
    if (n <= 4) {
        static const seq_t init[5] = {0, 4, 22, 133, 813};
        return init[n];
    }

    /* --- Initialise cache once --------------------------------------- */
    if (cache == NULL) {
        /* Allocate an initial capacity large enough for the request. */
        capacity = (size_t)n + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* memory allocation failure */

        /* initialise the first five entries (index 0 unused). */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)22;
        cache[3] = (seq_t)133;
        cache[4] = (seq_t)813;
        cached_max = 4;
    }

    /* --- Grow the cache if necessary --------------------------------- */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* memory allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* --- Compute any missing terms up to n --------------------------- */
    if (n > cached_max) {
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)6
                       + cache[i - 2]
                       - cache[i - 3] * (seq_t)2;
        }
        cached_max = n;
    }

    return cache[n];
}
