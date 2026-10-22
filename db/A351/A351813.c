/*
 * A351813  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - x)^3) / (1 - x).
 * https://oeis.org/A351813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0}^{n-1} binomial(n+2k-1, n-k-1) * a(k)
 *            (n≥1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   alloc_seq_array, binomial, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array holding a(0)…a(n) */
    static size_t  cap    = 0;      /* physical size of cache */
    static size_t  sz     = 0;      /* number of values actually stored */

    /* Ensure room for a(n) */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache)                /* allocation failed – return 0 */
            return (seq_t)0;
        /* copy old data */
        for (size_t i = 0; i < cap; ++i)
            new_cache[i] = cache[i];
        /* initialise rest (not needed, but harmless) */
        for (size_t i = cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        /* free previous block if any */
        if (cache)
            free_seq_array(cache);
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise a(0) once */
    if (sz == 0) {
        cache[0] = (seq_t)1;
        sz = 1;
    }

    /* Compute missing values up to n */
    for (size_t i = sz; i <= (size_t)n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < (idx_t)i; ++k) {
            seq_t c = binomial((idx_t)i + 2 * k - 1,
                               (idx_t)i - k - 1);
            sum += cache[(size_t)k] * c;
        }
        cache[i] = sum;
    }
    sz = (size_t)n + 1;

    return cache[(size_t)n];
}
