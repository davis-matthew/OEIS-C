/*
 * A278476  a(n) = floor((1 + sqrt(2))^3*a(n-1)) for n>0, a(0) = 1.
 * https://oeis.org/A278476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 13*a(n-2) - a(n-3) a(0) = 1, a(1) = 14, a(2)
 *            = 196
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array holding pre‑computed terms   */
    static size_t cap   = 0;         /* allocated size of the array       */
    static size_t last  = 2;         /* index of the last computed term   */

    /* --- first call: allocate and initialise the first three terms --- */
    if (!cache) {
        size_t req = (size_t)n + 1;
        if (req < 3) req = 3;                /* always at least 3 slots     */
        cap   = req;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)196;
        /* 'last' remains 2 – we already have indices 0,1,2 available   */
    }

    /* --- grow the cache if the requested index is not yet covered --- */
    size_t req = (size_t)n + 1;
    if (req > cap) {
        if (cap == 0) cap = 3;
        while (cap < req) cap <<= 1;                /* double until enough   */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* --- compute missing terms using the linear recurrence --- */
    if (n > last) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)15 * cache[i - 1]
                     - (seq_t)13 * cache[i - 2]
                     - cache[i - 3];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
