/*
 * A367973  Expansion of e.g.f. exp(exp(x) - 1)/(1 - 2*x).
 * https://oeis.org/A367973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1..n} (2^k * (k-1)! + 1) * C(n-1,k-1) * a(n-k), a(0)
 *            = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Return the n‑th term of OEIS sequence A367973.  */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static idx_t   max_n = 0;   /* highest n already computed */
    static size_t  cap   = 0;   /* size of allocated arrays */
    static seq_t  *A     = NULL;   /* sequence values   */
    static seq_t  *F     = NULL;   /* factorials        */
    static seq_t  *P2    = NULL;   /* powers of two     */

    /* Ensure capacity for all indices up to n. */
    if (A == NULL || n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);

        /* Reallocate arrays; realloc handles NULL gracefully. */
        A  = (seq_t *)realloc(A,  new_cap * sizeof(seq_t));
        F  = (seq_t *)realloc(F,  new_cap * sizeof(seq_t));
        P2 = (seq_t *)realloc(P2, new_cap * sizeof(seq_t));

        /* Initialise new factorials and powers of two. */
        if (cap == 0) {
            /* Base values for 0. */
            F[0]  = 1;   /* 0!   */
            P2[0] = 1;   /* 2^0  */
            A[0]  = 1;   /* a(0) */
            cap   = 1;
        }
        for (size_t i = cap; i < new_cap; ++i) {
            if (i == 0) continue;  /* already initialised */
            F[i]  = F[i - 1] * (seq_t)i;         /* i!   */
            P2[i] = P2[i - 1] * (seq_t)2;        /* 2^i  */
        }
        cap = new_cap;
    }

    /* Compute new terms up to n. */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        seq_t sum  = 0;
        seq_t binom = 1;                /* C(cur-1,0) */
        /* k goes from 1 to cur. */
        for (idx_t k = 1; k <= cur; ++k) {
            /* Compute f = 2^k * (k-1)! + 1 */
            seq_t f = P2[k] * F[k - 1];
            f = f + (seq_t)1;

            /* term = f * binom * a[cur-k] */
            seq_t term = f;
            term = term * binom;
            term = term * A[(size_t)(cur - k)];
            sum = sum + term;

            /* Update binomial for the next k. */
            if (k < cur) {
                binom = binom * (seq_t)(cur - k);
                binom = binom / (seq_t)k;
            }
        }
        A[(size_t)cur] = sum;
    }

    if (n > max_n) max_n = n;
    return A[(size_t)n];
}
