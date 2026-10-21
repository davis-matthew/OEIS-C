/*
 * A263416  a(n) = Product_{k=0..n} (3*k+1)^(n-k).
 * https://oeis.org/A263416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n} (3*k+1)^(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence
 *   a(0) = 1
 *   a(n) = a(n-1) * Π_{k=0..n-1} (3*k+1)   for n≥1   */

/* NOTE: For strategies requiring an arbitrary‑precision type the
 * code allocates the helper arrays with C++'s `new[]` to invoke
 * constructors; for plain C integral types it uses `malloc`.  */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    static seq_t *A = NULL;   /* cached a values      */
    static seq_t *P = NULL;   /* cached cumulative products  */
    static size_t cap = 0;    /* current allocated capacity */

    size_t need = (size_t)n + 1;   /* we store values 0 .. n */

    if (need > cap) {
        /* allocate larger storage */
        size_t new_cap = need;
#if defined(__cplusplus)
        seq_t *newP = new seq_t[new_cap];
        seq_t *newA = new seq_t[new_cap];
#else
        seq_t *newP = (seq_t *)malloc(new_cap * sizeof(seq_t));
        seq_t *newA = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
        /* allocation failure handling */
        if (!newP || !newA) {
#if defined(__cplusplus)
            delete[] newP;
            delete[] newA;
#else
            free(newP);
            free(newA);
#endif
            return (seq_t)0;      /* error indicator */
        }

        /* copy any previously computed entries */
        for (size_t i = 0; i < cap; ++i) {
            newP[i] = P[i];
            newA[i] = A[i];
        }

        /* initialise base case if this is the first allocation */
        if (cap == 0) {
            newP[0] = (seq_t)1;
            newA[0] = (seq_t)1;
        }

        /* compute new entries */
        size_t start = (cap == 0) ? 1 : cap;
        for (size_t i = start; i < need; ++i) {
            newP[i] = newP[i - 1] * ((seq_t)(3 * i + 1));
            newA[i] = newA[i - 1] * newP[i - 1];
        }

        /* release old storage */
#if defined(__cplusplus)
        delete[] P;
        delete[] A;
#else
        free(P);
        free(A);
#endif
        /* update static data */
        P   = newP;
        A   = newA;
        cap = new_cap;
    }

    return A[(size_t)n];
}
