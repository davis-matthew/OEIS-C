/*
 * A284520  a(1) = 1, a(2) = a(3) = a(4) = 2; a(n) = a(a(n-1)-1) +
 *          a(n-a(n-3)-1) for n > 4.
 * https://oeis.org/A284520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(a(n-1)-1) + a(n-a(n-3)-1) a(1) = 1, a(2) = a(3) = a(4)
 *            = 2
 * Range:     u8 n <= 508, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (0‑based array) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (>=needed) */
    static size_t computed = 0;    /* highest index already computed (1‑based) */

    /* grow cache if necessary */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while ((idx_t)new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);           /* allocation failure */
        /* initialise new slots to 0 */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms that have not yet been computed */
    for (idx_t i = (idx_t)computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 1;                     /* a(1) = 1 */
        } else if (i <= 4) {
            cache[i-1] = 2;                   /* a(2)=a(3)=a(4)=2 */
        } else {
            /* indices in recurrence are 1‑based; cache is 0‑based */
            idx_t idx1 = (idx_t)cache[i-2] - 1;                /* a(i-1)-1  (1‑based) */
            idx_t idx2 = (idx_t)(i - cache[i-4] - 1);          /* i-a(i-3)-1 (1‑based) */

            /* safety: idx1 and idx2 must be ≥ 1 */
            cache[i-1] = cache[idx1-1] + cache[idx2-1];
        }
    }
    computed = (size_t)n;
    return cache[(size_t)n - 1];
}
