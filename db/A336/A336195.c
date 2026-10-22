/*
 * A336195  a(0) = 1; a(n) = Sum_{k=0..n-1} binomial(n,k)^3 * a(k).
 * https://oeis.org/A336195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0}^{n-1} binomial(n,k)^3 * a(k) (n >= 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap = 0;     /* allocated size */
    static idx_t last = 0;     /* last index computed (>= 0) */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 1;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* memory exhaustion */
        cache[0] = (seq_t)1;                  /* a(0) = 1 */
    }

    /* Grow cache if necessary */
    while (cap <= (size_t)n) {
        size_t newcap = cap * 2;
        if (newcap == 0) newcap = 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* memory exhaustion */
        cache = tmp;
        cap = newcap;
    }

    /* Compute values up to n if not already done */
    while (last < n) {
        idx_t i = last + 1;          /* index to compute a(i) */
        seq_t sum = (seq_t)0;

        /* Compute binomial coefficients for row i on the fly */
        seq_t bin = (seq_t)1;        /* C(i,0) */
        for (idx_t k = 0; k < i; ++k) {
            /* term = bin^3 * a(k) */
            seq_t b2 = bin * bin;
            seq_t b3 = b2 * bin;
            seq_t term = b3 * cache[k];
            sum += term;

            /* update bin to C(i, k+1) if needed */
            if (k + 1 < i) {
                seq_t num = (seq_t)(i - k);
                seq_t den = (seq_t)(k + 1);
                bin = (bin * num) / den;
            }
        }

        cache[i] = sum;
        last = i;
    }

    return cache[n];
}
