/*
 * A400343  a(0) = 1; a(2*n) = Sum_{j=0..n} a(j) * a(n-j), a(2*n+1) = 2 *
 *          a(2*n).
 * https://oeis.org/A400343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(2*n) = Σ_{j=0}^{n} a(j)*a(n-j); a(2*n+1) = 2*a(2*n)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation ------------------------------------------------ */
    static seq_t *cache = NULL;   /* array of a(0) .. a(cap-1) */
    static idx_t  cap   = 0;      /* number of valid entries           */
    static idx_t  maxi  = 0;      /* highest index already computed    */

    /* First call: initialise cache for a(0) ------------------------------- */
    if (cache == NULL) {
        cap  = 1;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        maxi = 0;
    }

    /* Grow cache if needed -------------------------------------------------- */
    if (n >= cap) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        /* zero‑initialize new entries ------------------------------------- */
        for (idx_t i = cap; i <= n; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = n + 1;
    }

    /* Compute missing values up to n ------------------------------------- */
    if (n > maxi) {
        for (idx_t i = maxi + 1; i <= n; ++i) {
            if ((i & 1ULL) == 0) {               /* even index 2*k */
                idx_t k   = i >> 1;
                seq_t sum = (seq_t)0;
                for (idx_t j = 0; j <= k; ++j) {
                    seq_t prod = cache[j] * cache[k - j];
                    sum += prod;
                }
                cache[i] = sum;
            } else {                              /* odd index 2*k+1 */
                cache[i] = cache[i - 1] << 1;
            }
        }
        maxi = n;
    }

    return cache[n];
}
