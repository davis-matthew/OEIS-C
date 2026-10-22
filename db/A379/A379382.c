/*
 * A379382  G.f. A(x) satisfies A(x) = sqrt( (1 + 2*x*A(x))/(1 - 2*x*A(x)^2)
 *          ).
 * https://oeis.org/A379382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A379382
   Recurrence derived from the functional equation
   A(x) = sqrt((1 + 2*x*A(x))/(1 - 2*x*A(x)^2)).
   The coefficients a(n) satisfy
      a(0) = 1,
      for n ≥ 1
        c[n] = 2 * ( Σ_{k=0}^{n-1} c[k] * c[n-1-k] + a[n-1] )
        a[n] = ( c[n] - Σ_{k=1}^{n-1} a[k] * a[n-k] ) / 2
   where c[n] = Σ_{k=0}^{n} a[k] * a[n-k].
   These relations are integer and produce the required sequence. */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build system.
   Use them directly; do not redefine them. */

seq_t a(idx_t n)
{
    /* Static memoised arrays for the sequence values a[0..max_n]
       and the auxiliary convolution values c[0..max_n]. */
    static seq_t *A = NULL;          /* a(n) values               */
    static seq_t *C = NULL;          /* c(n) = Σ a(i)*a(n-i)      */
    static idx_t max_n = 0;          /* highest n computed so far */

    /* Initialise storage if this is the first call. */
    if (A == NULL) {
        /* Allocate space for index 0 only. */
        size_t init_size = 1;
        A = (seq_t *)malloc(init_size * sizeof(seq_t));
        C = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!A || !C) {
            /* Out of memory – aborting is fine; the caller will get 0. */
            return 0;
        }
        A[0] = (seq_t)1;          /* a(0) = 1                */
        C[0] = (seq_t)1;          /* c(0) = a(0)*a(0) = 1    */
        max_n = 0;
    }

    /* If the requested index has not yet been computed, extend the
       arrays and compute successive terms up to n. */
    if (n > max_n) {
        idx_t i;
        /* Reallocate arrays to hold indices 0 .. n. */
        size_t new_size = (size_t)(n + 1);
        A = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        C = (seq_t *)realloc(C, new_size * sizeof(seq_t));
        if (!A || !C) {
            /* Reallocation failed – abort. */
            return 0;
        }

        /* Compute terms from max_n+1 up to n. */
        for (i = max_n + 1; i <= n; ++i) {

            /* Compute c[i] = 2 * ( Σ_{k=0}^{i-1} c[k]*c[i-1-k] + a[i-1] ). */
            seq_t sum_c2 = 0;
            idx_t k;
            for (k = 0; k <= i - 1; ++k) {
                sum_c2 += C[k] * C[i - 1 - k];
            }
            /* Note: a[i-1] is already known because i-1 <= max_n. */
            seq_t ci = 2 * (sum_c2 + A[i - 1]);

            /* Compute sum of products of a's excluding the two a[i] terms. */
            seq_t sum_a_prod = 0;
            for (k = 1; k <= i - 1; ++k) {
                sum_a_prod += A[k] * A[i - k];
            }

            /* Finally compute a[i] = (c[i] - sum_a_prod) / 2. */
            seq_t ai = (ci - sum_a_prod) / 2;

            /* Store computed values. */
            A[i] = ai;
            C[i] = ci;
        }
        max_n = n;
    }

    return A[n];
}
