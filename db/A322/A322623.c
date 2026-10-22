/*
 * A322623  E.g.f.: (1 + sinh(x)) / (1 - sinh(x)).
 * https://oeis.org/A322623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>=1: a(n) = 2 + Σ_{k odd, 1≤k<n} C(n,k) * a(n-k)
 *            where C(n,k) is the binomial coefficient
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS sequence A322623 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of g[n] = a(n)/2 for n>0 */
    static seq_t *g = NULL;          /* array of g values */
    static idx_t g_len = 0;          /* highest index currently computed */
    static size_t g_cap = 0;         /* allocated capacity */

    /* Base case n==0 */
    if (n == 0)
        return (seq_t)1;

    /* Initialise cache if needed */
    if (g == NULL) {
        g_cap = 1;
        g = (seq_t *)malloc(g_cap * sizeof(seq_t));
        if (!g) /* allocation failure should be handled by caller */
            abort();
        g[0] = (seq_t)1; /* g[0] = 1 */
        g_len = 0;
    }

    /* Expand cache to cover requested n */
    if (n > g_len) {
        /* Grow capacity */
        size_t new_cap = g_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        g = (seq_t *)realloc(g, new_cap * sizeof(seq_t));
        if (!g)
            abort();
        g_cap = new_cap;

        /* Compute missing g values */
        for (idx_t i = g_len + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;   /* running sum for g[i] */
            seq_t cur = (seq_t)1;   /* binom(i,0) = 1 */

            for (idx_t k = 1; k <= i; ++k) {
                /* Update binomial coefficient: cur = C(i,k) */
                cur = (cur * (i - k + 1)) / k;

                /* Only odd k contribute and k < i (k == i handled separately) */
                if ((k & 1U) && k < i) {
                    seq_t prod = cur * g[i - k];
                    sum += prod;
                }
            }

            /* If i is odd, add the constant term from k==i */
            if (i & 1U)
                sum += (seq_t)1;

            g[i] = sum;
        }
        g_len = n;
    }

    /* Return a(n) = 2 * g[n]  (for n >= 1) */
    return g[n] * (seq_t)2;
}
