/*
 * A364375  G.f. satisfies A(x) = (1 + x*A(x)) * (1 - x*A(x)^3).
 * https://oeis.org/A364375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 22, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A364375 generated from the functional equation
   A(x) = (1 + x*A(x)) * (1 - x*A(x)^3).
   Equivalent recurrence:
      a(0) = 1
      For n ≥ 1:
          a(n) = a(n-1) 
                 - coeff(A^3, x^{n-1}) 
                 - coeff(A^4, x^{n-2})
   where  coeff(A^3, x^k) = Σ_{i=0}^{k} a(i)·a2(k-i)
         coeff(A^4, x^k) = Σ_{i=0}^{k} a(i)·a3(k-i)
   with a2 and a3 defined as ordinary convolutions.
   The implementation precomputes all values up to the largest
   requested index, using O(n²) integer arithmetic.
*/

#include <stddef.h>
#include <stdlib.h>

/* The types seq_t and idx_t are supplied by the build system. */
static seq_t *a_arr   = NULL;  /* a(n) values                */
static seq_t *a2_arr  = NULL;  /* convolution a * a          */
static seq_t *a3_arr  = NULL;  /* convolution a * a2         */
static seq_t *a4_arr  = NULL;  /* convolution a * a3         */
static size_t  cap    = 0;     /* current allocated capacity */
static idx_t   len    = 0;     /* highest computed index          */

/* Grow the allocation until 'need' indices can be stored. */
static void
ensure_capacity(idx_t need)
{
    if ((size_t)need < cap) return;
    size_t new_cap = cap ? cap * 2 : 8;
    if (new_cap <= (size_t)need) new_cap = (size_t)need + 1;
    a_arr   = (seq_t *)realloc(a_arr,   new_cap * sizeof(seq_t));
    a2_arr  = (seq_t *)realloc(a2_arr,  new_cap * sizeof(seq_t));
    a3_arr  = (seq_t *)realloc(a3_arr,  new_cap * sizeof(seq_t));
    a4_arr  = (seq_t *)realloc(a4_arr,  new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* Compute all a(n) up to index 'n' if not yet done. */
static void
compute_up_to(idx_t n)
{
    if (a_arr == NULL) {                 /* first call: initialise */
        ensure_capacity(1);
        a_arr[0] = 1;                    /* a(0) = 1                */
        a2_arr[0] = 1;                   /* a2[0] = a0^2 = 1        */
        a3_arr[0] = a_arr[0] * a2_arr[0];/* a3[0] = a0*a2[0] = 1    */
        a4_arr[0] = a_arr[0] * a3_arr[0];/* a4[0] = a0*a3[0] = 1    */
        len = 0;
    }

    if (n <= len) return;                /* already computed          */

    ensure_capacity(n);

    for (idx_t m = len + 1; m <= n; ++m) {
        /* Compute a(m) from previous terms. */
        seq_t c3 = a3_arr[m - 1];
        seq_t c4 = (m >= 2) ? a4_arr[m - 2] : (seq_t)0;
        a_arr[m] = a_arr[m - 1] - c3 - c4;

        /* a2[m] = Σ_{i=0..m} a[i] * a[m-i] */
        seq_t sum2 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum2 += a_arr[i] * a_arr[m - i];
        }
        a2_arr[m] = sum2;

        /* a3[m] = Σ_{i=0..m} a[i] * a2[m-i] */
        seq_t sum3 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum3 += a_arr[i] * a2_arr[m - i];
        }
        a3_arr[m] = sum3;

        /* a4[m] = Σ_{i=0..m} a[i] * a3[m-i] */
        seq_t sum4 = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sum4 += a_arr[i] * a3_arr[m - i];
        }
        a4_arr[m] = sum4;
    }

    len = n;
}

/* Public sequence accessor. */
extern seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_arr[n];
}
