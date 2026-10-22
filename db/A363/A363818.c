/*
 * A363818  G.f. satisfies A(x) = (1 + x/A(x)^2)/(1 - x)^2.
 * https://oeis.org/A363818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363818
   Defined implicitly by the algebraic equation
   (1 - 2x + x^2)·A(x)^3 - A(x)^2 - x = 0.
   The series coefficients a(n) satisfy
   a(0) = 1
   for n ≥ 1
       a(n) = -S_d(n) + S_c(n) + 2·D(n-1) - D(n-2) + (n==1 ? 1 : 0)

   where
       S_c(n) = ∑_{k=1}^{n-1} a(k)·a(n-k)         (known part of C_n)
       S_d(n) = S_c(n) + ∑_{k=1}^{n-1} a(k)·C(n-k)  (known part of D_n)
       C_n   = S_c(n) + 2·a(n)
       D_n   = S_d(n) + 3·a(n)
   and C_n = ∑_{k=0}^n a(k)·a(n-k),
         D_n = ∑_{k=0}^n a(k)·C_{n-k}.

   The recurrence derives from equating coefficients in the defining
   equation.  The code below builds the sequence incrementally,
   memoising computed terms so that calls in any order remain fast. */

#include <stdlib.h>

static seq_t *seq_a = NULL;   /* a[n] */
static seq_t *seq_c = NULL;   /* C_n = Σ a(k)·a(n-k)  */
static seq_t *seq_d = NULL;   /* D_n = Σ a(k)·C_{n-k}  */
static idx_t  max_computed = (idx_t)-1;   /* last index computed */

static void compute_terms(idx_t n)
{
    if (seq_a == NULL) { /* initialise to size n+1 */
        seq_a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        seq_c = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        seq_d = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; ++i) {
            seq_a[i] = (seq_t)0;
            seq_c[i] = (seq_t)0;
            seq_d[i] = (seq_t)0;
        }
        max_computed = 0;
        /* base case n = 0 */
        seq_a[0] = (seq_t)1;
        seq_c[0] = (seq_t)1;   /* 1*1 */
        seq_d[0] = (seq_t)1;   /* 1*1 */
    } else if (n > max_computed) {   /* grow existing arrays */
        size_t old_cap = (size_t)(max_computed + 1);
        size_t new_cap = (size_t)(n + 1);
        seq_a = (seq_t *)realloc(seq_a, new_cap * sizeof(seq_t));
        seq_c = (seq_t *)realloc(seq_c, new_cap * sizeof(seq_t));
        seq_d = (seq_t *)realloc(seq_d, new_cap * sizeof(seq_t));
        for (idx_t i = old_cap; i <= n; ++i) {
            seq_a[i] = (seq_t)0;
            seq_c[i] = (seq_t)0;
            seq_d[i] = (seq_t)0;
        }
    }
    /* compute new terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t known_c = (seq_t)0;   /* Σ_{k=1}^{i-1} a(k)*a(i-k) */
        seq_t known_d = (seq_t)0;   /* known part of D_i (see above) */
        if (i >= 2) {
            for (idx_t k = 1; k <= i - 1; ++k) {
                known_c += seq_a[k] * seq_a[i - k];
            }
        }
        known_d = known_c;                                 /* add known_c part */
        for (idx_t k = 1; k <= i - 1; ++k) {
            known_d += seq_a[k] * seq_c[i - k];
        }
        seq_t delta = (i == 1) ? (seq_t)1 : (seq_t)0;
        seq_t temp1 = seq_d[i - 1] + seq_d[i - 1];   /* 2*D_{i-1} */
        seq_t temp2 = (i >= 2) ? seq_d[i - 2] : (seq_t)0;
        seq_t a_i = -known_d + known_c + temp1 - temp2 + delta;
        seq_a[i] = a_i;
        seq_c[i] = known_c + (a_i + a_i);          /* 2*a_i */
        seq_d[i] = known_d + (a_i + a_i + a_i);    /* 3*a_i */
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    compute_terms(n);
    return seq_a[n];
}
