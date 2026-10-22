/*
 * A335982  Expansion of e.g.f. exp(4 * (1 - exp(-x)) + x).
 * https://oeis.org/A335982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(n) = a(n-1) + 4 * Sum_{k=0}^{n-1} (-1)^(n-k-1) *
 *            binomial(n-1,k) * a(k)
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 15, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Memoised computation */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;   /* pointer to array a[0..last] */
    static idx_t last = 0;        /* highest index currently stored */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, sizeof(seq_t));
        if (cache == NULL)  /* allocation failure – fallback to first term */
            return (seq_t)1;
        cache[0] = (seq_t)1;  /* a(0) = 1 */
        last = 0;
    }

    /* If the requested term is already cached, return it */
    if (n <= last)
        return cache[n];

    /* Extend cache to size n+1 */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)          /* allocation failure – return last known value */
        return cache[last];
    cache = tmp;

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binomial(i-1,0) */

        for (idx_t k = 0; k < i; ++k) {
            seq_t prod = binom * cache[k];

            /* Apply the sign (-1)^(i-k-1) */
            if ((((i - 1) - k) & (idx_t)1) != 0)
                sum -= prod;
            else
                sum += prod;

            /* Compute next binomial coefficient binomial(i-1,k+1) if needed */
            if (k + 1 < i)
                binom = binom * ((seq_t)((i - 1) - k)) / (seq_t)(k + 1);
        }

        cache[i] = cache[i - 1] + (seq_t)4 * sum;
    }

    last = n;
    return cache[n];
}
