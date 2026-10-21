/*
 * A258902  Expansion of e.g.f. Series_Reversion(x - x^2/2 - x^3/3).
 * https://oeis.org/A258902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A258902
   Recurrence:
        19*a(n) = 21*(2*n-3)*a(n-1) + 4*(3*n-5)*(3*n-7)*a(n-2)
   Initial values:
        a(1) = 1 , a(2) = 1
   Indexing: n ≥ 1 (a(0) is undefined – the function returns 0 for it). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static idx_t cache_size = 0;   /* highest index currently cached (0 means empty) */
    static seq_t *cache   = NULL;   /* array for values, cache[0] unused */

    if (n == (idx_t)0)
        return 0;   /* undefined for n < 1 */

    /* Grow cache to hold up to index n (including index 0, which we leave unused). */
    if (n > cache_size) {
        /* Reallocate (or allocate first time) */
        cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!cache)
            abort();   /* allocation failure */

        /* Compute new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == (idx_t)1 || i == (idx_t)2) {
                cache[i] = (seq_t)1;
                continue;
            }

            /* Coefficients of the recurrence – all positive for i ≥ 3 */
            int64_t c1 = 21LL * ((int64_t)i * 2 - 3);          /* 21*(2i-3) */
            int64_t c2 = 4LL  * ((int64_t)i * 3 - 5) *
                                   ((int64_t)i * 3 - 7);        /* 4*(3i-5)*(3i-7) */

            /* Use 128‑bit intermediate to avoid overflow in 64‑bit builds */
            unsigned __int128 term1 = (unsigned __int128)cache[i - 1] * (unsigned __int128)c1;
            unsigned __int128 term2 = (unsigned __int128)cache[i - 2] * (unsigned __int128)c2;
            unsigned __int128 sum   = term1 + term2;

            cache[i] = (seq_t)(sum / (unsigned __int128)19);
        }
        cache_size = n;
    }

    return cache[n];
}
