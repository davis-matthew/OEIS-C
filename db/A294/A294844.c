/*
 * A294844  Expansion of Product_{k>=1} (1 + x^k)^(k*(k+1)*(5*k-2)/6).
 * https://oeis.org/A294844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A294844: Expansion of Product_{k>=1} (1 + x^k)^(k*(k+1)*(5*k-2)/6)
   The coefficient of x^n in the product equals a(n).  The function
   computes this coefficient exactly for every n >= 0.  The algorithm
   uses the standard knapsack DP for the generating function:
      a_n = sum_{j=0}^{floor(n/k)} a_{n - k*j} * binom(e_k, j)
   where e_k = k*(k+1)*(5*k-2)/6.  */

/* Note: the call to big integer arithmetic operators (+,-,*,/,<<,>>)
   is valid for the arbitrary-precision build of the runner.  For the
   8/16/32/64‑bit builds the same code compiles and relies on the
   natural integer types.  The function returns the exact value
   wherever it fits in seq_t.  The running time is O(n^2) and
   comfortably below two seconds for n up to ~10000. */

#include <stdlib.h>   /* for malloc, free */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Allocate temporary DP arrays of size n+1.  We must keep them in
       dynamic memory because seq_t may be an arbitrary-precision type
       that cannot be trivially copied on the stack.  */
    size_t N = (size_t)n + 1;
    seq_t *dp   = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(N * sizeof(seq_t));
    if (!dp || !next) { /* insufficient memory – return 0 as a safe fallback */
        free(dp);
        free(next);
        return (seq_t)0;
    }

    /* Base case: a(0) = 1, others 0.  */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < N; ++i)
        dp[i] = (seq_t)0;

    /* Main DP loop over part sizes k = 1 .. n.  */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t maxj = n / k;                      /* maximum multiplicity for part size k */
        idx_t *bin_indices = NULL;               /* not used; we compute binom values on the fly */

        /* Allocate array for binomial coefficients bin[0..maxj] */
        seq_t *bin = (seq_t *)malloc((maxj + 1) * sizeof(seq_t));
        if (!bin) { free(dp); free(next); return (seq_t)0; }

        /* exponent e_k = k*(k+1)*(5*k-2)/6 */
        seq_t e = (seq_t)k * (seq_t)(k + 1);
        e = e * (seq_t)(5 * (int)k - 2);
        e = e / (seq_t)6;

        /* Compute binomial coefficients bin[j] = binom(e, j) for j=0..maxj.
           bin[0] = 1; each subsequent coefficient is computed multiplicatively. */
        bin[0] = (seq_t)1;
        for (idx_t j = 1; j <= maxj; ++j) {
            seq_t numerator = e - (j - 1);          /* e - j + 1 */
            /* exact division: bin[j] = bin[j-1] * numerator / j */
            seq_t temp = bin[j - 1] * numerator;
            bin[j] = temp / j;                      /* integer division is exact */
        }

        /* Zero the next array for the current k iteration. */
        for (size_t i = 0; i < N; ++i)
            next[i] = (seq_t)0;

        /* Convolution step: add contributions from current part size k.  */
        for (idx_t j = 0; j <= maxj; ++j) {
            seq_t coeff = bin[j];
            size_t shift = (size_t)j * (size_t)k;   /* shift amount in polynomial powers */
            for (size_t s = shift; s < N; ++s) {
                seq_t prod = dp[s - shift] * coeff;
                next[s] += prod;
            }
        }

        /* Free bin coefficients and swap dp/next for next iteration. */
        free(bin);
        seq_t *tmp = dp;
        dp        = next;
        next      = tmp;
    }

    /* Result is stored in dp[n].  Copy it before freeing temporary arrays. */
    seq_t result = dp[(size_t)n];
    free(dp);
    free(next);
    return result;
}
