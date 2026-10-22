/*
 * A346077  a(n) = 1 + Sum_{k=1..n-5} a(k) * a(n-k-5).
 * https://oeis.org/A346077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n-5} a(k) * a(n-k-5) Initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=a(5)=1
 * Range:     u8 n <= 20, u16 n <= 34, u32 n <= 61, u64 n <= 113,
 *            u128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoization cache */
    static seq_t *dp   = NULL;   /* array of computed terms   */
    static size_t  cap  = 0;     /* current allocation size   */
    static idx_t   done = 0;     /* number of terms stored    */

    /* first allocation and initial values */
    if (dp == NULL) {
        cap  = 16;
        dp   = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!dp) return (seq_t)0;          /* out of memory */
        for (idx_t i = 0; i < 6; ++i) dp[i] = 1; /* a(0)..a(5) */
        for (idx_t i = 6; i < cap; ++i) dp[i]   = 0;
        done = 6;
    }

    /* grow the cache if the requested index is beyond the current capacity */
    if ((idx_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out of memory */
        dp = tmp;
        for (idx_t i = done; i < new_cap; ++i) dp[i] = 0;
        cap = new_cap;
    }

    /* return the value if already computed */
    if (n < done)
        return dp[n];

    /* compute all missing terms up to n */
    for (idx_t i = done; i <= n; ++i) {
        if (i <= 5) {             /* safety for i ≤ 5  */
            dp[i] = 1;
            continue;
        }
        seq_t sum = 0;
        idx_t limit = i - 5;
        for (idx_t k = 1; k <= limit; ++k)
            sum += dp[k] * dp[i - k - 5];
        dp[i] = sum + (seq_t)1;
    }

    done = n + 1;                  /* now dp[0..n] are valid */
    return dp[n];
}
