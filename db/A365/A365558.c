/*
 * A365558  Expansion of e.g.f. 1 / (4 - 3 * exp(x))^(2/3).
 * https://oeis.org/A365558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * ∑_{k=1..n} (3*n – k) * C(n,k) * a(n–k) a(0) = 1.
 *            Recurrence derived from the e.g.f. of the sequence
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   free_seq, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (last index + 1) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* initialise cache with a(0)=1 */
    if (cache_len == 0) {
        cache_cap = 16;                   /* initial capacity */
        cache    = seq_alloc_new_array(cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* enlarge cache if necessary (grow by factors of two) */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap > 0 ? cache_cap << 1 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];
        free_seq(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;   /* binomial coefficient C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient C(i,k) */
            bin = bin * (seq_t)(i - k + 1) / (seq_t)k;

            /* term = a[i–k] * bin * (3*i – k) */
            seq_t term = cache[i - k];
            term = term * bin;
            term = term * ((seq_t)3 * i - (seq_t)k);

            sum = sum + term;
        }

        cache[i] = sum / (seq_t)i;
    }

    cache_len = n + 1;
    return cache[n];
}
