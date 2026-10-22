/*
 * A357668  Expansion of e.g.f. sinh( 3 * (exp(x) - 1) )/3.
 * https://oeis.org/A357668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊(n−1)/2⌋} 9ᵏ · Stirling₂(n,2k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Computes the n‑th term of OEIS A357668 */
seq_t a(idx_t n)
{
    /* static state */
    static idx_t      max_computed = 0;   /* highest n already computed */
    static size_t     cap          = 0;   /* current array capacity */
    static seq_t    *S            = NULL;/* Stirling numbers of the current row */
    static seq_t    *cache        = NULL;/* cached sequence values */

    /* initialise data structures the first time */
    if (max_computed == 0 && cap == 0) {
        cap = 1;
        S = (seq_t *)malloc(cap * sizeof(seq_t));
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!S || !cache) {
            abort();                     /* allocation failure */
        }
        S[0]    = 1;          /* S(0,0)=1 */
        cache[0] = 0;         /* a(0)=0 */
    }

    /* grow the arrays if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *newS = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        seq_t *newcache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newS || !newcache) {
            abort();                     /* allocation failure */
        }
        /* initialise new elements */
        for (size_t i = cap; i < new_cap; ++i) {
            newS[i] = 0;
            newcache[i] = 0;
        }
        S = newS;
        cache = newcache;
        cap = new_cap;
    }

    /* compute rows up to n if needed */
    for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
        /* Update Stirling numbers for this new row in place
           (k iterated downward) */
        for (idx_t k = cur; k >= 1; --k) {
            S[k] = S[k - 1] + (seq_t)k * S[k];
        }
        S[0] = 0;                     /* S(cur,0)=0 for cur>0 */

        /* Accumulate the required value a(cur) */
        seq_t sum = 0;
        idx_t mmax = (cur - 1) / 2;
        for (idx_t m = mmax;; --m) {
            idx_t k = 2 * m + 1;       /* odd index */
            sum = sum * 9 + S[k];
            if (m == 0) break;
        }
        cache[cur] = sum;
    }
    max_computed = n;

    return cache[n];
}
