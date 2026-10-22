/*
 * A383262  Expansion of e.g.f. f(x)^2 * exp(f(x)) / 2, where f(x) = (exp(3*x)
 *          - 1)/3.
 * https://oeis.org/A383262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} 3^(n-k) * binomial(k,2) * Stirling2(n,k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values and supporting tables. */
    static idx_t   max_n = 0;          /* highest index computed */
    static size_t  cap   = 2;          /* current capacity of arrays */

    static seq_t *S      = NULL;   /* Stirling numbers S[k] for current n */
    static seq_t *pow3   = NULL;   /* 3^i for i < cap */
    static seq_t *a_cache= NULL;   /* cached sequence values */

    /* Initialise on first call. */
    if (S == NULL) {
        S      = (seq_t*)calloc(cap, sizeof(seq_t));
        pow3   = (seq_t*)calloc(cap, sizeof(seq_t));
        a_cache= (seq_t*)calloc(cap, sizeof(seq_t));
        if (!S || !pow3 || !a_cache)
            return (seq_t)0;  /* allocation failed */

        pow3[0] = (seq_t)1;          /* 3^0 */
        a_cache[0] = (seq_t)0;       /* a(0) = 0 */
        a_cache[1] = (seq_t)0;       /* a(1) = 0 */
    }

    /* Return cached result if already available. */
    if (n <= max_n)
        return a_cache[n];

    /* Grow arrays if needed. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        S      = (seq_t*)realloc(S,      new_cap * sizeof(seq_t));
        pow3   = (seq_t*)realloc(pow3,   new_cap * sizeof(seq_t));
        a_cache= (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!S || !pow3 || !a_cache)
            return (seq_t)0;          /* allocation failed */
        for (size_t i = cap; i < new_cap; ++i) {
            S[i]      = (seq_t)0;
            pow3[i]   = (seq_t)0;
            a_cache[i]= (seq_t)0;
        }
        cap = new_cap;
    }

    /* Compute values up to n, filling the cache. */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {

        /* Update Stirling numbers S(k) for current n=cur.
           In-place recurrence (from high to low):
           S[n][k] = k * S[n-1][k] + S[n-1][k-1]   for k=1..n-1
           S[n][n] = 1
           S[0] is kept zero.  */
        for (idx_t k = cur; k >= 1; --k) {
            if (k == cur) {
                S[k] = (seq_t)1;
            } else {
                /* use old S[k] and old S[k-1] since we loop from high to low */
                S[k] = (seq_t)k * S[k] + S[k-1];
            }
        }
        S[0] = (seq_t)0;

        /* Pre‑compute the power of three for this index. */
        pow3[cur] = pow3[cur-1] * (seq_t)3;

        /* Compute a(cur) from its defining sum. */
        seq_t sum = (seq_t)0;
        for (idx_t k = 2; k <= cur; ++k) {
            seq_t term = S[k];
            /* binomial(k,2) = k*(k-1)/2 */
            seq_t bin = (seq_t)k * (k-1) / (seq_t)2;
            term = term * bin;
            term = term * pow3[cur - k];
            sum += term;
        }
        a_cache[cur] = sum;
    }

    max_n = n;
    return a_cache[n];
}
