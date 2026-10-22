/*
 * A354735  a(0) = a(1) = 1; a(n) = 4 * Sum_{k=0..n-2} a(k) * a(n-k-2).
 * https://oeis.org/A354735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A354735
   a(0) = a(1) = 1
   a(n) = 4 * Σ_{k=0}^{n-2} a(k) * a(n-k-2)   for n ≥ 2        */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values. */
    static seq_t *cache = NULL;      /* array of computed terms           */
    static size_t cache_cap = 0;     /* allocated capacity (in elements)  */
    static idx_t cache_len = 0;      /* number of terms already known     */

    /* Grow the cache so that indices up to n are addressable. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 4;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;          /* double until sufficient       */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {          /* unlikely in the testing harness*/
            abort();
        }
        /* Zero‑initialise any newly added entries. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to and including n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = (seq_t)1;
        } else {
            idx_t m = i - 2;          /* need a(k) * a(m-k) for k=0..m */
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j <= m / 2; ++j) {
                seq_t prod = cache[j] * cache[m - j];
                if (j == m - j) {
                    sum += prod;      /* centre term, counted once   */
                } else {
                    sum += prod + prod;/* symmetric pair, counted twice */
                }
            }
            cache[i] = (seq_t)4 * sum;
        }
    }
    cache_len = n + 1;   /* all indices up to n are now computed */

    return cache[n];
}
