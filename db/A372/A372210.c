/*
 * A372210  Product of n!, n-th Pell number and n-th harmonic number.
 * https://oeis.org/A372210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n!*Pell(n)*H(n) = Pell(n) * sum_{k=1}^n (n!)/k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return A372210(a(n)) */
seq_t a(idx_t n)
{
    /* static caches for the sequence, its factorials, and Pell numbers    */
    static seq_t *S = NULL;   /* S[i] = sum_{k=1}^i (i!)/k                      */
    static seq_t *P = NULL;   /* Pell numbers: P[0]=0,P[1]=1                   */
    static idx_t cur = 0;     /* largest n already computed                    */
    static seq_t fact = 0;    /* current factorial (cur!)                      */

    if (n == 0) {            /* offset starts at 1; 0 is undefined */
        return (seq_t)0;
    }

    /* lazy allocation / extending of the caches                            */
    if (n > cur) {
        size_t newSize = (size_t)(n + 1);   /* indices 0..n */
        S = (seq_t *)realloc(S, newSize * sizeof(seq_t));
        P = (seq_t *)realloc(P, newSize * sizeof(seq_t));

        if (cur == 0) {                  /* initialise base cases */
            /* indices 0 unused; place zero to avoid undefined behaviour */
            S[0] = (seq_t)0;
            P[0] = (seq_t)0;

            /* n = 1 */
            S[1] = (seq_t)1;            /* 1!/1 = 1                           */
            P[1] = (seq_t)1;            /* Pell(1) = 1                        */
            fact = (seq_t)1;            /* 1!                                  */
            cur = 1;
        }

        for (idx_t i = cur + 1; i <= n; ++i) {
            seq_t fact_prev = fact;                 /* (i-1)!           */
            fact = fact * (seq_t)i;                 /* i!                */
            P[i] = (P[i-1] * (seq_t)2) + P[i-2];    /* Pell recurrence   */
            S[i] = (seq_t)i * S[i-1] + fact_prev;   /* sum recurrence    */
        }
        cur = n;
    }

    /* a(n) = Pell(n) * sum_{k=1}^n (n!)/k = P[n] * S[n] */
    return P[n] * S[n];
}
