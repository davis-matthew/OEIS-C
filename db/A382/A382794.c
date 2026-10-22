/*
 * A382794  a(n) = Sum_{k=0..n} Stirling1(n,k) * Stirling2(n,k) * (k!)^2.
 * https://oeis.org/A382794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0}^{n} Stirling1(n,k) * Stirling2(n,k) * (k!)^2
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 12, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


static seq_t *A = NULL;          /* a(n) values               */
static seq_t *fac = NULL;        /* factorials k!             */
static seq_t *S1_prev = NULL;    /* Stirling1(n-1,k)          */
static seq_t *S1_curr = NULL;    /* Stirling1(n,k)            */
static seq_t *S2_prev = NULL;    /* Stirling2(n-1,k)          */
static seq_t *S2_curr = NULL;    /* Stirling2(n,k)            */
static idx_t   nmax = 0;         /* highest n already computed */
static size_t  cap  = 0;         /* current allocated capacity */

/* Increase the allocated capacity if needed.  All arrays are resized
   in place; new elements are zero‑initialized.  The cast from void*
   to seq_t* is legal in both C and C++ because it is an explicit
   cast. */
static void ensure_capacity(idx_t newsize)
{
    if (newsize <= cap) return;

    /* Reallocate each array to the requested size */
    A        = (seq_t*)realloc(A,    newsize * sizeof(seq_t));
    fac      = (seq_t*)realloc(fac,  newsize * sizeof(seq_t));
    S1_prev  = (seq_t*)realloc(S1_prev, newsize * sizeof(seq_t));
    S1_curr  = (seq_t*)realloc(S1_curr, newsize * sizeof(seq_t));
    S2_prev  = (seq_t*)realloc(S2_prev, newsize * sizeof(seq_t));
    S2_curr  = (seq_t*)realloc(S2_curr, newsize * sizeof(seq_t));

    /* Initialise any newly allocated space to zero */
    for (idx_t i = cap; i < newsize; ++i) {
        fac[i] = 0;
        S1_prev[i] = 0;
        S1_curr[i] = 0;
        S2_prev[i] = 0;
        S2_curr[i] = 0;
    }
    cap = newsize;
}

/* Compute a(n) using recurrence relations for Stirling numbers.
   All calculations are done in the type seq_t, which may be an
   arbitrary‑precision integer class. */
seq_t a(idx_t n)
{
    /* Lazy static initialisation on the first call */
    if (A == NULL) {
        ensure_capacity(1);
        A[0] = 1;          /* a(0) = 1 */
        fac[0] = 1;        /* 0!  = 1 */
        S1_prev[0] = 1;    /* Stirling1(0,0) = 1 */
        S2_prev[0] = 1;    /* Stirling2(0,0) = 1 */
        nmax = 0;
    }

    /* Compute successive rows until the requested n is reached */
    if (n > nmax) {
        ensure_capacity(n + 1);

        for (idx_t i = nmax + 1; i <= n; ++i) {
            /* Factorial i! = (i-1)! * i */
            fac[i] = fac[i - 1] * (seq_t)i;

            /* Stirling numbers of the first kind:
               S1_curr[k] = S1_prev[k-1] - (i-1)*S1_prev[k]  */
            S1_curr[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (seq_t)(i - 1) * S1_prev[k];
                seq_t term2 = S1_prev[k - 1];
                S1_curr[k] = term2 - term1;
            }

            /* Stirling numbers of the second kind:
               S2_curr[k] = k*S2_prev[k] + S2_prev[k-1]     */
            S2_curr[0] = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (seq_t)k * S2_prev[k];
                seq_t term2 = S2_prev[k - 1];
                S2_curr[k] = term1 + term2;
            }

            /* Sum over k to obtain a(i) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {   /* k=0 contributes 0 */
                seq_t t = S1_curr[k] * S2_curr[k];
                t *= fac[k];
                t *= fac[k];
                sum += t;
            }
            A[i] = sum;

            /* Prepare for the next iteration by swapping the buffers */
            seq_t *tmp = S1_prev;  S1_prev = S1_curr; S1_curr = tmp;
            tmp        = S2_prev;  S2_prev = S2_curr; S2_curr = tmp;
        }
        nmax = n;
    }
    return A[n];
}
