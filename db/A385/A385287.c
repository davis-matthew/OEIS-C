/*
 * A385287  a(0) = 1, a(1) = 0; a(n) = a(n-2) + Sum_{k=0..n-1} k * a(k) *
 *          a(n-1-k).
 * https://oeis.org/A385287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A385287
   a(0) = 1, a(1) = 0
   a(n) = a(n-2) + Σ_{k=0..n-1} k * a(k) * a(n-1-k)   (n ≥ 2) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *A = NULL;      /* a(i) values */
    static seq_t *C = NULL;      /* k * a(k) values */
    static idx_t last_computed = 0; /* highest index for which A[i] is valid */

    /* Base cases – returned without touching the cache */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Resize and initialise storage if required */
    if (n > last_computed) {
        idx_t new_size = n + 1;
        /* realloc returns void*; cast explicitly to work in C++ */
        A = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        C = (seq_t *)realloc(C, new_size * sizeof(seq_t));
        if (!A || !C) abort();   /* allocation failure – caller can handle */

        /* initialise base indices when not already present */
        if (last_computed == 0) {
            A[0] = (seq_t)1;
            C[0] = (seq_t)0;     /* 0 * a(0) */
        }
        if (last_computed < 1) {
            A[1] = (seq_t)0;
            C[1] = (seq_t)0;     /* 1 * a(1) = 0 */
            last_computed = 1;
        }

        /* Compute new terms up to n */
        for (idx_t m = last_computed + 1; m <= n; ++m) {
            seq_t sum = 0;
            /* Σ_{k=0..m-1} (k * a(k)) * a(m-1-k) */
            for (idx_t k = 0; k < m; ++k) {
                sum += C[k] * A[m - 1 - k];
            }
            A[m] = A[m - 2] + sum;
            C[m] = (seq_t)m * A[m];
        }
        last_computed = n;
    }

    return A[n];
}
