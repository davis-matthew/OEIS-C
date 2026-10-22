/*
 * A361571  Expansion of e.g.f. exp( (x * (1+x))^3 ).
 * https://oeis.org/A361571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} binomial(3k, n-3k)/k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* --- Factorial cache ------------------------------------------ */
    static seq_t *fac = NULL;
    static size_t fac_size = 0;   /* number of factorials stored   */

    if (fac == NULL) {                /* First call: allocate slot for 0!   */
        fac = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!fac) return (seq_t)0;    /* OOM guard                      */
        fac[0] = 1;
        fac_size = 1;
    } else if (fac_size <= n) {       /* Grow factorial array if needed   */
        fac = (seq_t*)realloc(fac, (n + 1) * sizeof(seq_t));
        if (!fac) return (seq_t)0;    /* OOM guard                      */
        for (idx_t i = fac_size; i <= n; ++i)
            fac[i] = fac[i - 1] * (seq_t)i;
        fac_size = (size_t)n + 1;
    }

    /* --- Sequence cache -------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* largest index computed + 1    */

    if (cache == NULL) {             /* First call: allocate cache    */
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* OOM guard                      */
        cache_size = 0;
    } else if (cache_size < n + 1) {  /* Grow cache if needed          */
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* OOM guard                      */
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {                 /* a(0) = 1                      */
            cache[0] = 1;
            continue;
        }

        seq_t fac_i = fac[i];         /* i!                            */
        seq_t sum = 0;

        for (idx_t k = 0; 3 * k <= i; ++k) {
            idx_t m = i - 3 * k;      /* m = n-3k                       */
            seq_t bin = binomial(3 * k, m);        /* binom(3k, m) */
            seq_t temp = fac_i / fac[k];           /* i! / k! */
            sum += temp * bin;                      /* add term   */
        }

        cache[i] = sum;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
