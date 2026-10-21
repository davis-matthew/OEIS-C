/*
 * A293429  a(0) = 0; and for n > 0, a(n) = a(n-1) + (A008966(4n-1) -
 *          A008966(4n+1)).
 * https://oeis.org/A293429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0; for n > 0, a(n) = a(n-1) + (A008966(4n-1) –
 *            A008966(4n+1))
 * Range:     i8 n <= 33, i16 n <= 8495, i32 n <= 9801, i64 n <= 9801,
 *            i128 n <= 9801, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008966(m) = 1 if m is square‑free, 0 otherwise. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* memoization buffer */
    static idx_t cached_n    = (idx_t)-1; /* highest n computed so far */
    static idx_t cache_size  = 0;     /* current buffer size */

    /* One‑time initialization (C++ cast to seq_t* required) */
    if (cached_n == (idx_t)-1) {
        cache_size = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – should not occur */
        cache[0] = (seq_t)0;           /* a(0) = 0 */
        cached_n = 0;
    }

    /* If the requested value is already cached, return it */
    if (n <= cached_n)
        return cache[n];

    /* Grow the buffer if needed (doubling strategy) */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        if (new_size < cache_size * 2)
            new_size = cache_size * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return cache[cached_n]; /* fallback on allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all missing terms up to n */
    for (idx_t k = cached_n + 1; k <= n; ++k) {
        seq_t x1 = (seq_t)(4 * k - 1);
        seq_t x2 = (seq_t)(4 * k + 1);

        int delta = (is_squarefree(x1) ? 1 : 0)
                  - (is_squarefree(x2) ? 1 : 0);

        cache[k] = cache[k - 1] + (seq_t)delta;
    }

    cached_n = n;
    return cache[n];
}
