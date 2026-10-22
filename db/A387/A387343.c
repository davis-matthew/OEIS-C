/*
 * A387343  Expansion of 1/(1 - 8*x + 4*x^2)^(5/2).
 * https://oeis.org/A387343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A387343 – Expansion of 1/(1 - 8*x + 4*x^2)^(5/2)
 *
 * Recurrence (for n > 1):
 *   n * a(n) = 4 * (2*n + 3) * a(n-1) - 4 * (n + 3) * a(n-2)
 *   a(0) = 1,  a(1) = 20
 *
 * The implementation memoises computed values up to the requested index.
 * For 64‑bit or smaller seq_t types the intermediate products are performed
 * in unsigned __int128 (when available) to avoid overflow.
 * For larger seq_t types (arbitrary‑precision) the operations are performed
 * directly on seq_t, which supports the usual arithmetic operators.
 *
 * No external libraries are required; only standard <stdlib.h> and
 * <stdint.h> are used.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static size_t len  = 0;   /* number of values stored           */
    static size_t cap  = 0;   /* allocated capacity                */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap  = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)20;
        len = 2;
    }

    /* grow cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0; /* allocation failed */
        cache = new_cache;
        cap   = new_cap;
    }

    /* compute new terms up to n */
    for (idx_t i = (idx_t)len; i <= n; ++i) {
        if (i <= 1) continue;  /* a(0) and a(1) already known */

        /* recurrence:  a(i) = (4*(2*i+3)*a(i-1) - 4*(i+3)*a(i-2)) / i */
        if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller: use __int128 */
            unsigned __int128 t1 =
                (unsigned __int128)4 *
                (unsigned __int128)(2 * i + 3) *
                (unsigned __int128)cache[i - 1];
            unsigned __int128 t2 =
                (unsigned __int128)4 *
                (unsigned __int128)(i + 3) *
                (unsigned __int128)cache[i - 2];

            cache[i] = (seq_t)((t1 - t2) / i);
        } else {                           /* arbitrary‑precision type */
            seq_t t1 = cache[i - 1];
            t1 *= (seq_t)(2 * i + 3);
            t1 *= (seq_t)4;

            seq_t t2 = cache[i - 2];
            t2 *= (seq_t)(i + 3);
            t2 *= (seq_t)4;

            cache[i] = (t1 - t2) / (seq_t)i;
        }
    }

    len = (size_t)(n + 1);
    return cache[n];
}
