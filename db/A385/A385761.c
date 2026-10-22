/*
 * A385761  G.f. A(x) satisfies A(x) = 1/((1 - x) * (1 - x*A(x) -
 *          x^6*A'''''(x))).
 * https://oeis.org/A385761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} (1 + 24*k - 50*k^2 + 35*k^3 - 10*k^4 +
 *            k^5) * a(k) * a(n-1-k), with a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static caches for sequence values and coefficient values */
    static seq_t *A   = NULL;
    static idx_t Alen = 0;
    static seq_t *C   = NULL;
    static idx_t Clen = 0;

    /* initialise caches on first call */
    if (A == NULL) {
        A   = (seq_t *)malloc(sizeof(seq_t));
        A[0] = (seq_t)1;   /* a(0) */
        Alen = 1;

        C   = (seq_t *)malloc(sizeof(seq_t));
        C[0] = (seq_t)1;   /* coefficient for k = 0 */
        Clen = 1;
    }

    /* extend cache if necessary */
    if (n >= Alen) {
        idx_t old_len = Alen;
        size_t new_A_size = (size_t)(n + 1);
        A = (seq_t *)realloc(A, new_A_size * sizeof(seq_t));
        Alen = (idx_t)new_A_size;

        /* compute missing coefficients */
        if (n >= Clen) {
            size_t new_C_size = (size_t)n;   /* need indices 0 .. n-1 */
            C = (seq_t *)realloc(C, new_C_size * sizeof(seq_t));
            for (idx_t k = Clen; k < n; ++k) {
                __int128 kk = k;
                __int128 val =
                      (__int128)1
                    + (__int128)24 * kk
                    - (__int128)50 * kk * kk
                    + (__int128)35 * kk * kk * kk
                    - (__int128)10 * kk * kk * kk * kk
                    + kk * kk * kk * kk * kk;
                C[k] = (seq_t)val;
            }
            Clen = n;
        }

        /* compute missing sequence terms */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t prod = A[k] * A[i - 1 - k];
                sum += C[k] * prod;
            }
            A[i] = (seq_t)1 + sum;
        }
    }

    return A[n];
}
