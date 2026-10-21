/*
 * A278681  Pisot sequence T(3,16).
 * https://oeis.org/A278681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) + 2*a(n-3) - 2*a(n-4); Initial
 *            values: a(0)=3, a(1)=16, a(2)=85, a(3)=451
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases for the first four terms */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)85;
    if (n == 3) return (seq_t)451;

    static seq_t *cache    = NULL;   /* cached sequence terms */
    static idx_t last_max = (idx_t)-1; /* highest index computed so far */
    static size_t cap     = 0;       /* current allocated capacity */

    /* Initialize the cache on the first call */
    if (cache == NULL) {
        cap     = 4;
        cache   = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)85;
        cache[3] = (seq_t)451;
        last_max = 3;
    }

    /* If the requested index is already computed, return it */
    if (n <= last_max) {
        return cache[n];
    }

    /* Ensure the cache has enough capacity up to index n */
    while (cap <= n) {
        size_t new_cap = (cap == 0) ? 4 : (cap * 2);
        seq_t *temp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!temp) { /* allocation failure – should not happen in tests */
            return (seq_t)0;
        }
        cache = temp;
        cap   = new_cap;
    }

    /* Compute terms up to index n using the linear recurrence */
    for (idx_t i = last_max + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i-1]
                 - 4 * cache[i-2]
                 + 2 * cache[i-3]
                 - 2 * cache[i-4];
    }

    last_max = n;
    return cache[n];
}
