/*
 * A250061  Number of length 2+6 0..n arrays with no seven consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining five terms.
 * https://oeis.org/A250061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) for n >
 *            9, with a(1) = 21, a(2) = 1299, …, a(9) = 59084865 (computed
 *            from an explicit polynomial)
 * Range:     u8 none, u16 n <= 2, u32 n <= 15, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   poly_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/poly_a.h"

/* --- Main sequence function --- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n <= 0 */

    /* Static cache for previous results */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;     /* allocated size (indices 0 .. cap-1) */
    static idx_t  last  = 0;     /* greatest n already computed */

    /* Initial allocation */
    if (!cache) {
        /* Enough room for the first 9 terms plus a few safety slots */
        cap = (n > 10) ? n + 1 : 20;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
    }

    /* Grow cache if requested n exceeds current capacity */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        if (new_cap < cap * 2) new_cap = cap * 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        /* zero newly allocated part (calloc left earlier blocks zeroed) */
        for (idx_t i = cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 9) {
            val = poly_a(i);  /* use polynomial for first 9 terms */
        } else {
            seq_t v = (seq_t)0;
            v += cache[i-1] * (seq_t)9;
            v -= cache[i-2] * (seq_t)36;
            v += cache[i-3] * (seq_t)84;
            v -= cache[i-4] * (seq_t)126;
            v += cache[i-5] * (seq_t)126;
            v -= cache[i-6] * (seq_t)84;
            v += cache[i-7] * (seq_t)36;
            v -= cache[i-8] * (seq_t)9;
            v += cache[i-9] * (seq_t)1;
            val = v;
        }
        cache[i] = val;
    }

    last = (last > n) ? last : n;
    return cache[n];
}
