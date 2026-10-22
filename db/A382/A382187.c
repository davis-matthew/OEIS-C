/*
 * A382187  Expansion of 1/(1 - 4 * Sum_{k>=0} x^(2^k))^(1/2).
 * https://oeis.org/A382187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A382187: coefficients of 1/(1-4∑_{k≥0}x^{2^k})^{1/2}
   via the recurrence
       C[0]=1,  for n>0   C[n] = 4·∑_{p=1,2,4,… ≤ n} C[n-p]
       a[0]=1,  a[n] = (C[n] - ∑_{k=1}^{n-1} a[k]·a[n-k]) / 2
   Here C[n] = ∑_{i=0}^n a[i]·a[n-i] is the convolution of the
   sequence with itself.  The function is memoised so calls are
   order‑independent. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *A    = NULL;
    static seq_t *C    = NULL;
    static size_t cap  = 0;          /* allocated capacity */
    static idx_t  last = (idx_t)-1;  /* highest index computed */

    /* first call: initialise a(0) & c(0) */
    if (last == (idx_t)-1) {
        cap  = 1;
        A    = (seq_t *)malloc(cap * sizeof(seq_t));
        C    = (seq_t *)malloc(cap * sizeof(seq_t));
        A[0] = (seq_t)1;
        C[0] = (seq_t)1;
        last = 0;
    }

    /* already known? */
    if (n <= last)
        return A[n];

    /* grow arrays to fit up to n (inclusive) */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap > 0 ? cap : 1;
        while (newcap < need)
            newcap <<= 1;
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        C = (seq_t *)realloc(C, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute new terms from last+1 … n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute C[i] = 4 * sum_{p powers of two ≤ i} C[i-p] */
        seq_t sumC = (seq_t)0;
        for (idx_t p = 1; p <= i; p <<= 1) {
            sumC += C[i - p];
        }
        C[i] = sumC * (seq_t)4;

        /* compute sum_{k=1}^{i-1} a[k]*a[i-k] */
        seq_t prodSum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            prodSum += A[k] * A[i - k];
        }

        /* deduce a[i] */
        A[i] = (C[i] - prodSum) / (seq_t)2;
    }

    last = n;
    return A[n];
}
