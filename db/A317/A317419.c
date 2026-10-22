/*
 * A317419  a(n) = number of k with 1 <= k <= n-1 such that a(k) AND a(n-k) =
 *          0 (where AND denotes the bitwise AND operator).
 * https://oeis.org/A317419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k (1 ≤ k ≤ n-1) such that a(k) AND a(n-k) == 0
 * Range:     u8 n <= 1317, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0; for n > 1, a(n) = Σ_{k=1}^{n-1} [ (a(k)&a(n-k)) == 0 ] */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;        /* cache of computed terms */
    static idx_t cache_cap = 0;      /* current allocated capacity (indexable 0..cap) */
    static idx_t max_computed = 0;   /* highest n for which a(n) is computed */

    /* Ensure the cache is large enough for n */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed: terminate */
        seq = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if not yet done */
    if (max_computed == 0) {
        seq[1] = (seq_t)0;
        max_computed = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t cnt = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            if ((seq[k] & seq[i - k]) == (seq_t)0) {
                ++cnt;
            }
        }
        seq[i] = cnt;
    }

    if (n > max_computed) max_computed = n;
    return seq[n];
}
