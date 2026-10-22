/*
 * A375948  Expansion of e.g.f. 1 / (3 - 2 * exp(x))^(3/2).
 * https://oeis.org/A375948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n-1) + 2 * Σ_{j=0}^{n-1} C(n,j)·a(j) (n>0) Recurrence
 *            for auxiliary b: b(0)=1 b(n) = b(n-1) + Σ_{j=0}^{n-1}
 *            C(n-1,j)·a(j+1) (n>0)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, free, size_t */

seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *A = NULL;   /* a array   */
    static seq_t *B = NULL;   /* b array   */
    static idx_t  max_n = (idx_t)-1;  /* highest computed index   */
    static size_t cap   = 0;   /* allocated capacity        */

    /* initialise storage on first call */
    if (A == NULL) {
        cap = 1;
#ifdef __cplusplus
        A = new seq_t[cap];
        B = new seq_t[cap];
#else
        A = (seq_t*)malloc(cap * sizeof(seq_t));
        B = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
        A[0] = (seq_t)1;          /* a(0) */
        B[0] = (seq_t)1;          /* b(0) */
        max_n = 0;
    }

    /* extend to required n */
    if (n > max_n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
#ifdef __cplusplus
            /* use new/delete for class types */
            seq_t* newA = new seq_t[new_cap];
            seq_t* newB = new seq_t[new_cap];
            for (size_t i = 0; i < cap; ++i) {
                newA[i] = A[i];
                newB[i] = B[i];
            }
            delete[] A;
            delete[] B;
            A = newA;
            B = newB;
#else
            /* use realloc for primitive types */
            A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
            B = (seq_t*)realloc(B, new_cap * sizeof(seq_t));
#endif
            cap = new_cap;
        }

        for (idx_t i = max_n + (idx_t)1; i <= n; ++i) {
            /* ---- compute a(i) ------------------------------------- */
            seq_t sumA  = (seq_t)0;
            seq_t cA    = (seq_t)1;          /* C(i,0) */
            for (idx_t j = 0; j < i; ++j) {
                sumA += cA * A[j];
                if (j + 1 < i) {
                    cA = cA * (seq_t)(i - j) / (seq_t)(j + 1);
                }
            }
            A[i] = B[i - 1] + (seq_t)2 * sumA;

            /* ---- compute b(i) ------------------------------------- */
            seq_t sumShift = (seq_t)0;
            seq_t cB       = (seq_t)1;          /* C(i-1,0) */
            for (idx_t j = 0; j < i; ++j) {
                sumShift += cB * A[j + 1];
                if (j + 1 < i) {
                    cB = cB * (seq_t)(i - 1 - j) / (seq_t)(j + 1);
                }
            }
            B[i] = B[i - 1] + sumShift;
        }
        max_n = n;
    }

    return A[n];
}
