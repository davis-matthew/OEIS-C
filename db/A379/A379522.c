/*
 * A379522  Expansion of (1/x) * Series_Reversion( x / ( (1+x)^3 * (1+2*x)^3 )
 *          ).
 * https://oeis.org/A379522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..n} 2^k * binomial(3*(n+1),k) *
 *            binomial(3*(n+1),n-k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 quickly */
    if (n == 0) return (seq_t)1;

    /* N = 3*(n+1) */
    idx_t N = 3 * (n + 1);
    size_t len = (size_t)n + 1;

    /* Allocate array for C[0..n] = binomial(N, k) */
#ifdef __cplusplus
    seq_t *C = new seq_t[len];
#else
    seq_t *C = (seq_t *)malloc(len * sizeof(seq_t));
#endif
    if (!C) return (seq_t)0;   /* allocation failure, return 0 as a safe fallback */

    C[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        /* C(N,k) = C(N,k-1) * (N-k+1) / k */
        /* use seq_t arithmetic which is exact for built‑in and big‑int types */
        C[k] = C[k-1] * (seq_t)(N - k + 1) / k;
    }

    /* Accumulate the sum. Use 128‑bit only if seq_t is a 64‑bit builtin type. */
    seq_t result;
    if (sizeof(seq_t) == 8) {
        unsigned __int128 sum = 0;
        for (idx_t k = 0; k <= n; ++k) {
            /* term = 2^k * C[k] * C[n-k] */
            unsigned __int128 term = (unsigned __int128)pow2(k)
                                     * (unsigned __int128)C[k]
                                     * (unsigned __int128)C[n-k];
            sum += term;
        }
        result = (seq_t)(sum / (n + 1));
    } else {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= n; ++k) {
            seq_t term = pow2(k);
            term = term * C[k];
            term = term * C[n-k];
            sum += term;
        }
        result = sum / (n + 1);
    }

#ifdef __cplusplus
    delete[] C;
#else
    free(C);
#endif
    return result;
}
