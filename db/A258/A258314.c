/*
 * A258314  G.f. B(x) satisfies: B(x) = 1 + x*A(x)*C(x) where A(x) = B(x)*C(x)
 *          and C(x) = 1 + 2*x*A(x)*B(x).
 * https://oeis.org/A258314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = B[n] where B(x) satisfies * B(x) = 1 + x*A(x)*C(x) * A(x)
 *            = B(x)*C(x) * C(x) = 1 + 2*x*A(x)*B(x) * and the coefficient
 *            recurrence follows from these equations. * Initial values: B[0]
 *            = A[0] = C[0] = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>
#include "../utils/mul.h"

/* Multiplication helper for C++ builds.
 * Works for any seq_t that defines operator* (including big-integer types). */
#ifdef __cplusplus

#endif

seq_t a(idx_t n)
{
    static seq_t *B = NULL, *A = NULL, *C = NULL;
    static idx_t maxn = 0;          /* highest index computed (initially 0) */

    /* Allocate first element if first call */
    if (B == NULL) {
        B = (seq_t *)malloc(sizeof(seq_t));
        A = (seq_t *)malloc(sizeof(seq_t));
        C = (seq_t *)malloc(sizeof(seq_t));
        assert(B && A && C);
        B[0] = A[0] = C[0] = (seq_t)1;
        maxn = 0;
    }

    /* Extend tables if needed */
    if (n > maxn) {
        size_t newcap = (size_t)n + 1;
        B = (seq_t *)realloc(B, newcap * sizeof(seq_t));
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        C = (seq_t *)realloc(C, newcap * sizeof(seq_t));
        assert(B && A && C);

        for (idx_t k = maxn + 1; k <= n; ++k) {
            /* Compute C[k] = 2 * Σ_{i=0}^{k-1} A[i] * B[k-1-i] */
#ifdef __cplusplus
            seq_t sumC = 0;
            for (idx_t i = 0; i < k; ++i) sumC += mul(A[i], B[k - 1 - i]);
            C[k] = sumC << 1;
#else
            unsigned __int128 sumC = 0;
            for (idx_t i = 0; i < k; ++i)
                sumC += (unsigned __int128)A[i] * (unsigned __int128)B[k - 1 - i];
            C[k] = (seq_t)(sumC << 1);
#endif

            /* Compute B[k] = Σ_{i=0}^{k-1} A[i] * C[k-1-i] */
#ifdef __cplusplus
            seq_t sumB = 0;
            for (idx_t i = 0; i < k; ++i) sumB += mul(A[i], C[k - 1 - i]);
            B[k] = sumB;
#else
            unsigned __int128 sumB = 0;
            for (idx_t i = 0; i < k; ++i)
                sumB += (unsigned __int128)A[i] * (unsigned __int128)C[k - 1 - i];
            B[k] = (seq_t)sumB;
#endif

            /* Compute A[k] = Σ_{i=0}^{k} B[i] * C[k-i] */
#ifdef __cplusplus
            seq_t sumA = 0;
            for (idx_t i = 0; i <= k; ++i) sumA += mul(B[i], C[k - i]);
            A[k] = sumA;
#else
            unsigned __int128 sumA = 0;
            for (idx_t i = 0; i <= k; ++i)
                sumA += (unsigned __int128)B[i] * (unsigned __int128)C[k - i];
            A[k] = (seq_t)sumA;
#endif
        }
        maxn = n;
    }
    return B[n];
}
