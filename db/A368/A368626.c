/*
 * A368626  Expansion of g.f. A(x) satisfying A(x) = 1 + x*(A(x)^2 -
 *          A(-x)^2)/2 + x*(A(x)^3 + A(-x)^3)/2.
 * https://oeis.org/A368626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368626:  a(0)=1.  For n>=1,
   a(n) = Σ_{i=0}^{n-1} a(i)·a(n-1-i)          if n is even
          Σ_{i+j+k=n-1} a(i)·a(j)·a(k)          if n is odd
   The recurrence follows from the functional equation
   A(x)=1+x·(A(x)^2−A(−x)^2)/2+x·(A(x)^3+A(−x)^3)/2. */
#include <stddef.h>                       /* size_t */
#include "../utils/seq_alloc_new_array.h"  /* seq_alloc_new_array() */
#include "../utils/seq_free_array.h"       /* seq_free_array() */

seq_t a(idx_t n)
{
    static seq_t *A   = NULL;   /* a[0..]                       */
    static seq_t *C2  = NULL;   /* convolution A*A coeffs       */
    static seq_t *C3  = NULL;   /* convolution A^3 coeffs       */
    static size_t  cap  = 0;    /* allocated size               */
    static idx_t   used = 0;    /* next index to compute        */

    /* --- lazy initialization ----------------------------------------- */
    if (cap == 0) {
        cap  = 1;
        used = 1;
        A   = seq_alloc_new_array(cap);
        C2  = seq_alloc_new_array(cap);
        C3  = seq_alloc_new_array(cap);
        A[0]   = 1;
        C2[0]  = 1;
        C3[0]  = 1;
    }

    /* --- enlarge storage if necessary ------------------------------- */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap * 2;
        while (newcap < (size_t)n + 1)
            newcap *= 2;

        seq_t *newA  = seq_alloc_new_array(newcap);
        seq_t *newC2 = seq_alloc_new_array(newcap);
        seq_t *newC3 = seq_alloc_new_array(newcap);

        for (idx_t i = 0; i < used; ++i) {
            newA[i]  = A[i];
            newC2[i] = C2[i];
            newC3[i] = C3[i];
        }

        seq_free_array(A);
        seq_free_array(C2);
        seq_free_array(C3);

        A   = newA;
        C2  = newC2;
        C3  = newC3;
        cap = newcap;
    }

    /* --- compute missing terms ------------------------------------ */
    for (idx_t i = used; i <= n; ++i) {
        if (i == 0) {              /* already initialized */
            /* nothing */
        } else if ((i & 1) == 0) { /* even index */
            A[i] = C2[i-1];
        } else {                   /* odd index */
            A[i] = C3[i-1];
        }

        /* recompute convolution values for new index i */
        seq_t sumC2 = 0;
        for (idx_t j = 0; j <= i; ++j)
            sumC2 += A[j] * A[i-j];
        C2[i] = sumC2;

        seq_t sumC3 = 0;
        for (idx_t j = 0; j <= i; ++j)
            sumC3 += A[j] * C2[i-j];
        C3[i] = sumC3;
    }

    used = n + 1;
    return A[n];
}
