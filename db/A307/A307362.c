/*
 * A307362  Expansion of e.g.f. Sum_{j>=0} (exp(x) - 1)^j / Product_{k=1..j}
 *          (1 - k*(exp(x) - 1)).
 * https://oeis.org/A307362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * k! * Bell(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            a(0)=1, a(1)=1, a(2)=5, a(3)=43, ...              */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* n is 0-based; we will compute values up to n. */
    size_t len = (size_t)n + 1;

    /* Allocate working arrays. */
    seq_t *S     = (seq_t *)malloc(len * sizeof(seq_t));   /* Stirling numbers of the 2nd kind (current row)   */
    seq_t *U     = (seq_t *)malloc(len * sizeof(seq_t));   /* U(k) = k! * Bell(k)                              */
    seq_t *bell  = (seq_t *)malloc(len * sizeof(seq_t));   /* Bell numbers                                    */
    seq_t *fact  = (seq_t *)malloc(len * sizeof(seq_t));   /* factorials                                      */
    seq_t *a_val = (seq_t *)malloc(len * sizeof(seq_t));   /* to store a(k) for all k if needed               */

    /* Initialise to zero. */
    for (size_t i = 0; i < len; ++i) {
        S[i]   = 0;
        U[i]   = 0;
        bell[i]= 0;
        fact[i]= 0;
        a_val[i]=0;
    }

    /* Base values. */
    bell[0] = 1;                 /* Bell(0)   */
    fact[0] = 1;                 /* 0!        */
    S[0]    = 1;                 /* S(0,0)=1  */
    U[0]    = fact[0] * bell[0]; /* 0!*Bell(0)=1 */
    a_val[0] = U[0];             /* a(0)=1    */

    /* Compute Bell numbers and U(k) for 1..n. */
    for (size_t i = 1; i < len; ++i) {
        /* Bell(i) = sum_{k=0}^{i-1} C(i-1,k) * Bell(k)   */
        seq_t sum   = 0;
        seq_t binom = 1;                      /* C(i-1,0) */
        size_t m = i - 1;
        for (size_t k = 0; k <= m; ++k) {
            sum += binom * bell[k];
            if (k + 1 <= m) {
                /* binom = binom * (m - k) / (k + 1)   */
                binom = (binom * (seq_t)(m - k)) / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;

        /* factorial(i) */
        fact[i] = fact[i - 1] * (seq_t)i;

        /* U(i) = i! * Bell(i) */
        U[i] = fact[i] * bell[i];
    }

    /* Compute Stirling numbers row by row and accumulate a(n). */
    /* We only need the final row for a(n); intermediate rows are
       generated in place. */
    for (size_t row = 1; row <= n; ++row) {
        /* Update Stirling numbers of the second kind: */
        /* S(row,k) = k*S(row-1,k) + S(row-1,k-1)  (k >= 1) */
        for (size_t k = row; k >= 1; --k) {
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        S[0] = 0;
    }

    /* Compute a(n) = sum_{k=0}^n S[n][k] * U(k) */
    seq_t result = 0;
    for (size_t k = 0; k <= n; ++k) {
        result += S[k] * U[k];
    }

    /* Free memory */
    free(S);
    free(U);
    free(bell);
    free(fact);
    free(a_val);

    return result;
}
