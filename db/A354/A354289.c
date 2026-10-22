/*
 * A354289  Expansion of e.g.f. (1 + x)^(3/(1 - 3 * log(1+x))).
 * https://oeis.org/A354289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = \sum_{k=1}^{n} A335531(k) * binomial(n-1,k-1) *
 *            a(n-k), where A335531(k) is given by A335531(0) = 1, A335531(m)
 *            = \sum_{i=1}^{m} (-3)(-1)^i * binomial(m,i) * (i-1)! *
 *            A335531(m-i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below computes a(n) for any n >= 0.  It uses only the
   integer operations allowed in the constraints and relies on the
   arbitrary‑precision support of seq_t (if provided by the build).  */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate arrays for A335531 and a(n) values up to n.  */
    seq_t *A = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *an = (seq_t *)malloc((n + 1) * sizeof(seq_t));

    A[0] = (seq_t)1;      /* A335531(0) = 1 */
    an[0] = (seq_t)1;     /* a(0) = 1 */

    /* Compute the sequence A335531 up to n.  */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t pos = (seq_t)0;   /* sum of positive terms */
        seq_t neg = (seq_t)0;   /* sum of negative terms */

        seq_t fact = (seq_t)1;  /* (i-1)! for i = 1 */
        seq_t binom = (seq_t)m; /* binomial(m,1) */

        for (idx_t i = 1; i <= m; ++i) {
            /* term = 3 * binom * fact * A[m-i] */
            seq_t term = binom;
            term *= fact;
            term *= A[m - i];
            term *= (seq_t)3;

            if (i & 1) {          /* i is odd → positive contribution */
                pos += term;
            } else {              /* i is even → negative contribution */
                neg += term;
            }

            /* Prepare factorial and binomial for the next i.  */
            if (i < m) {
                fact *= (seq_t)i;                     /* (i)!  */
                binom = binom * (seq_t)(m - i);       /* C(m,i+1) numerator part */
                binom /= (seq_t)(i + 1);               /* binom(m,i+1) */
            }
        }
        /* Combine positive and negative parts: at the end pos >= neg. */
        seq_t result = pos;
        result -= neg;
        A[m] = result;
    }

    /* Compute the target sequence a(n) using the recurrence. */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;

        seq_t binom = (seq_t)1;  /* binomial(m-1,0) */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = A[k];
            term *= binom;
            term *= an[m - k];
            sum += term;

            if (k < m) {
                binom = binom * (seq_t)(m - k);  /* C(m-1,k+1) numerator part */
                binom /= (seq_t)k;               /* binom(m-1,k+1) */
            }
        }
        an[m] = sum;
    }

    seq_t result = an[n];

    free(A);
    free(an);
    return result;
}
