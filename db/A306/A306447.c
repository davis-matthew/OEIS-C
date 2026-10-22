/*
 * A306447  Number of (undirected) Hamiltonian cycles in the n-antiprism
 *          graph.
 * https://oeis.org/A306447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) + a(n-5)
 * Initial values: a(1)=3, a(2)=9, a(3)=16, a(4)=29, a(5)=56
 * Recurrence valid for n > 5.
 */
#include <stdlib.h>

/* External linkage, as required */
seq_t a(idx_t n)
{
    /* Static cache that grows as needed and remembers previously
     * computed terms.  The cache indices correspond directly to the
     * OEIS indexing (a(1) is at cache[1]).  Indices 0 are unused. */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;       /* current capacity (max valid index)     */
    static idx_t  fmax  = 0;       /* highest n that has been computed yet   */

    if (n < 1)
        return 0;                  /* undefined for n < 1, but safe to return */

    /* Allocate or grow the cache if necessary */
    if (cap < n) {
        idx_t new_cap = (cap > 0) ? cap : 16;
        while (new_cap < n)
            new_cap <<= 1;          /* double until it contains n */

        seq_t *tmp = (seq_t *)calloc((size_t)(new_cap + 1), sizeof(seq_t));
        if (!tmp)
            abort();               /* allocation failure */

        if (cache) {
            memcpy(tmp, cache, (size_t)(cap + 1) * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n only, using recurrence or
     * hard‑coded initial terms.  Subsequent calls reuse the cache. */
    for (idx_t i = fmax + 1; i <= n; ++i) {
        if (i == 1)          cache[i] = (seq_t)3;
        else if (i == 2)     cache[i] = (seq_t)9;
        else if (i == 3)     cache[i] = (seq_t)16;
        else if (i == 4)     cache[i] = (seq_t)29;
        else if (i == 5)     cache[i] = (seq_t)56;
        else                 /* recurrence for i > 5 */
            cache[i] = 3 * cache[i-1] - cache[i-2]
                       - 2 * cache[i-3] + cache[i-5];
    }
    fmax = n;

    return cache[n];
}
