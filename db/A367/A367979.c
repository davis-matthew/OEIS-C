/*
 * A367979  Expansion of e.g.f. exp(-x) / (2 - exp(3*x)).
 * https://oeis.org/A367979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} (3*k-1)^n / 2^(k+1) Recurrence: a(0)=1,
 *            a(n)=(-1)^n + sum_{k=1..n} binomial(n,k)*3^k*a(n-k) (n>=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   // for malloc, realloc

/* External linkage, exactly as requested. */
seq_t a(idx_t n)
{
    /* Cache of previously computed terms for memoisation.  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity  */
    static size_t cache_len = 0;   /* number of elements actually stored  */

    /* Ensure we have the 0‑th element. */
    if (cache_len == 0) {
        /* Start with capacity 1. */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* out of memory – return 0 */
        cache[0] = 1;                   /* a(0)=1  */
        cache_len = 1;
    }

    /* Resize cache if we need to compute terms up to n. */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms from the current cache length up to n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t sum = (i & 1ULL) ? 0 : 1;   /* start with (-1)^i contribution */
        if (i == 0) {                     /* a(0) already stored */
            cache[0] = 1;
            continue;
        }

        /* Multiplicative computation of the binomial coefficients
           and of the powers of 3.  */
        seq_t binom = (seq_t)i;           /* binomial(i,1) */
        seq_t pow3  = 3;                  /* 3^1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom(i,k) * 3^k * a(i-k) */
            seq_t term = binom * pow3 * cache[i - k];
            sum += term;

            /* Prepare binom and pow3 for the next k.  */
            if (k < i) {                 /* no need for the last update */
                binom = binom * (seq_t)(i - k);
                binom = binom / (seq_t)(k + 1);
                pow3  = pow3 * 3;
            }
        }

        /* Adjust for the (-1)^i term when i is odd. */
        if (i & 1ULL) {
            sum -= 1;
        }

        cache[i] = sum;
    }

    cache_len = n + 1;   /* conveniently reflect all computed terms */
    return cache[n];
}
