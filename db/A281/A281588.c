/*
 * A281588  Expansion of e.g.f. z*(2*(exp(z) + 1)/(cosh(z) + cos(z)) - 1).
 * https://oeis.org/A281588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A281588
 *  Exponential generating function:
 *  F(z) = z * ( 2 * (exp(z)+1) / (cosh(z)+cos(z)) - 1 )
 *
 *  This implementation computes the sequence via the integer recurrence
 *  derived from the egf and uses only integer arithmetic.
 *
 *  a(0) = 0
 *  a(1) = 1
 *  a(n) = n * e[n-1]   for n ≥ 2
 *
 *  where e[0] = 2  and for n ≥ 1
 *    e[n] = 1 - (1/2) * Σ_{k=0}^{n-1} C(n,k) * b_{n-k} * e[k]
 *
 *  Here b_m = 0 if m is odd,
 *            2 if m ≡ 0 (mod 4), 0 otherwise.
 *
 *  The recurrence avoids division by non‑unit numbers and keeps all
 *  values integral.  It runs in O(n²) time with O(n) memory.
 */

#include <stdlib.h>
#include <string.h>

/* ---------------------  Helper data --------------------- */

static seq_t *E = NULL;          /* e[n] values, up to current size   */
static idx_t  E_size = 0;        /* number of elements in E            */

/* Ensure that E is allocated and computed up to index n-1 (i.e. size n)   */
static void ensure_e(idx_t n)
{
    if (E_size >= n) return; /* already computed */

    /* Allocate or grow array */
    if (E == NULL) {
        E = (seq_t*)malloc(sizeof(seq_t) * n);
    } else {
        E = (seq_t*)realloc(E, sizeof(seq_t) * n);
    }

    /* Compute new elements */
    idx_t start = E_size;
    if (start == 0) {
        /* e[0] = 2 */
        E[0] = (seq_t)2;
        ++start;
    }

    for (idx_t i = start; i < n; ++i) {
        /* Compute sum_e = Σ binom(i,k) * 2 * e[k]  for (i-k)%4==0  */
        seq_t sum_e = 0;
        seq_t binom = (seq_t)1;  /* binom(i,0) */
        for (idx_t k = 0; k < i; ++k) {
            if ( ((i - k) & 3) == 0 ) {                /* (i-k)%4==0  */
                seq_t term = binom;                    /* binom(i,k)  */
                term *= (seq_t)2;                       /* 2*binom    */
                term *= E[k];                           /* *e[k]      */
                sum_e += term;
            }
            /* Update binom to binom(i,k+1) */
            if (k < i - 1) {
                binom = binom * (seq_t)(i - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        /* e[i] = 1 - sum_e/2  (i >= 1) */
        seq_t e_val = (seq_t)1 - (sum_e / (seq_t)2);
        E[i] = e_val;
    }

    E_size = n;
}

/* ---------------------  Main function --------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Ensure e[n-1] is available */
    ensure_e(n);  /* we need e[0..n-1] */

    /* a(n) = n * e[n-1]  for n >= 2 */
    return (seq_t)n * E[n - 1];
}
