/*
 * A367839  Expansion of e.g.f. 1/(2 + x - exp(3*x)).
 * https://oeis.org/A367839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -n * a(n-1) + Sum_{k=1..n} 3^k * binomial(n,k) * a(n-k).
 *            Initial value: a(0) = 1. Indexing follows OEIS: a(0) is the
 *            first term
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed sequence values */
    static seq_t *cache   = NULL;
    static idx_t  cache_cap   = 0;   /* capacity of cache array          */
    static idx_t  computed_len = 0;   /* number of entries that are valid */

    /* static cache for powers of 3 */
    static seq_t *pow3   = NULL;
    static idx_t  pow3_cap   = 0;   /* capacity of pow3 array          */
    static idx_t  pow3_len   = 0;   /* number of computed powers of 3  */

    /* enlarge cache array if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* enlarge powers‑of‑3 array if necessary */
    if (n >= pow3_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(pow3, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* out of memory */
        pow3 = tmp;
        pow3_cap = new_cap;
    }

    /* compute missing powers of 3 */
    if (pow3_len == 0) {               /* initialise 3^0 */
        pow3[0] = (seq_t)1;
        pow3_len = 1;
    }
    while (pow3_len <= n) {            /* grow to the needed exponent */
        pow3[pow3_len] = pow3[pow3_len - 1] * (seq_t)3;
        pow3_len++;
    }

    /* compute sequence values up to n if not yet computed */
    for (idx_t i = computed_len; i <= n; ++i) {
        if (i == 0) {                  /* a(0) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;        /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient to C(i,k) */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;

            /* 3^k * binomial(i,k) * a(i-k) */
            seq_t term = pow3[k] * binom * cache[i - k];
            sum += term;
        }

        seq_t prod = (seq_t)i * cache[i - 1];
        cache[i] = sum - prod;
    }

    computed_len = n + 1;   /* all indices up to n are now valid */
    return cache[n];
}
