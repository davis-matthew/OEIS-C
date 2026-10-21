/*
 * A261648  Expansion of Product_{k>=0} ((1+x^(2*k+1))/(1-x^(2*k+1)))^5.
 * https://oeis.org/A261648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in \prod_{k>=0}
 *            ((1+x^(2*k+1))/(1-x^(2*k+1)))^5. The function computes this
 *            coefficient iteratively by multiplying the generating function
 *            term-by-term
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* allocate and free utilities that work for both C and C++ */
#if defined(__cplusplus)
  #include <new>
#endif
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* dp arrays and coefficient array */
    seq_t *dp    = NULL;
    seq_t *newdp = NULL;
    seq_t *c     = NULL;

    /* allocate for size n+1 */
#if defined(__cplusplus)
    dp    = new seq_t[n + 1];
    newdp = new seq_t[n + 1];
    c     = new seq_t[n + 1];
#else
    dp    = (seq_t*)malloc(sizeof(seq_t)*(n + 1));
    newdp = (seq_t*)malloc(sizeof(seq_t)*(n + 1));
    c     = (seq_t*)malloc(sizeof(seq_t)*(n + 1));
#endif

    /* check allocation (no error handling required) */
    if (!dp || !newdp || !c) return (seq_t)0;

    /* initialize dp[0] = 1, others 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* precompute coefficient array c[k] = sum_{i=0..5, i<=k} C(5,i)*C(k-i+4,4) */
    static const seq_t C5[6] = {(seq_t)1, (seq_t)5, (seq_t)10, (seq_t)10, (seq_t)5, (seq_t)1};
    c[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i <= 5 && i <= k; ++i)
        {
            idx_t t = k - i;                     /* t >= 0 */
            /* C(t+4,4) = (t+4)(t+3)(t+2)(t+1)/24 */
            seq_t comb = ((seq_t)(t + 4) * (t + 3) * (t + 2) * (t + 1)) / 24;
            sum += C5[i] * comb;
        }
        c[k] = sum;
    }

    /* iterate over odd m = 1,3,5,...,n */
    for (idx_t m = 1; m <= n; m += 2)
    {
        /* copy current dp to newdp */
        for (idx_t i = 0; i <= n; ++i) newdp[i] = dp[i];

        /* for each current exponent e, add contributions of k copies of part m */
        for (idx_t e = 0; e <= n; ++e)
        {
            seq_t cur = dp[e];
            if (cur == 0) continue;
            idx_t maxk = (n - e) / m;
            for (idx_t k = 1; k <= maxk; ++k)
            {
                newdp[e + k * m] += cur * c[k];
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    /* result is dp[n] */
    seq_t result = dp[n];

    /* free memory */
#if defined(__cplusplus)
    delete[] dp;
    delete[] newdp;
    delete[] c;
#else
    free(dp);
    free(newdp);
    free(c);
#endif

    return result;
}
