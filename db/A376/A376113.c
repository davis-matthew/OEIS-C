/*
 * A376113  a(0) = 1; a(n) = (1/3) * Sum_{k=1..n} (4^k-1) * a(k-1) * a(n-k).
 * https://oeis.org/A376113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/3) * Σ_{k=1..n} (4^k – 1) * a(k-1) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are provided by the build environment */
seq_t a(idx_t n)
{
    /* static caches for computed values and powers of 4 */
    static idx_t cache_cap = 0;          /* capacity of arrays                    */
    static idx_t computed   = 0;          /* highest index already computed (0‑based) */
    static seq_t *a_cache   = NULL;       /* sequence values a[0..]                */
    static seq_t *pow4_cache = NULL;      /* powers of 4: pow4_cache[k] = 4^k      */

    /* allocate or grow the caches as needed */
    if (cache_cap < n + 1) {
        idx_t new_cap = n + 1;
        a_cache   = (seq_t *)realloc(a_cache,   new_cap * sizeof(seq_t));
        pow4_cache= (seq_t *)realloc(pow4_cache,new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* initialise a[0] and pow4[0] once */
    if (computed == 0) {
        a_cache[0]    = 1;
        pow4_cache[0] = 1;
        computed = 0;
    }

    /* pre‑compute additional powers of 4 */
    for (idx_t i = computed + 1; i <= n; ++i) {
        pow4_cache[i] = pow4_cache[i - 1] * (seq_t)4;
    }

    /* compute new sequence values */
    for (idx_t m = computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term   = pow4_cache[k] - (seq_t)1;          /* (4^k - 1)   */
            term *= a_cache[k - 1];                           /* * a(k-1)    */
            term *= a_cache[m - k];                           /* * a(n-k)    */
            sum += term;
        }
        a_cache[m] = sum / (seq_t)3;                          /* divide by 3 */
    }

    computed = n;  /* all indices up to n are now ready */
    return a_cache[n];
}
