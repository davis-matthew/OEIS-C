/*
 * A289449  a(n) is the number of permutations of length n that avoid the
 *          pattern 231 and the mesh pattern (12, 281) or the same sequence
 *          for the mesh patterns (12, 313), (12, 345), (12, 377), (12, 401),
 *          (12, 403), (12, 465), (12, 467).
 * https://oeis.org/A289449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A127156(n+1,1) - number of permutations of length n
 *            avoiding pattern 231 and mesh pattern (12,281)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n) {
    static seq_t *Catalan = NULL;
    static seq_t *Prefix  = NULL;
    static seq_t *W       = NULL;
    static seq_t *A       = NULL;
    static size_t maxn    = 0;        /* largest n that has been computed, last index stored */

    /* first call – initialise arrays for up to n (or at least 0) */
    if (Catalan == NULL) {
        size_t init = (n > 0 ? n + 1 : 1);
        Catalan = (seq_t*)malloc(init * sizeof(seq_t));
        Prefix  = (seq_t*)malloc(init * sizeof(seq_t));
        W       = (seq_t*)malloc(init * sizeof(seq_t));
        A       = (seq_t*)malloc(init * sizeof(seq_t));
        if (!Catalan || !Prefix || !W || !A) return (seq_t)0;   /* out of memory */

        Catalan[0] = (seq_t)1;
        Prefix[0]  = (seq_t)1;
        W[0]       = (seq_t)1;
        A[0]       = (seq_t)1;
        maxn = 0;
    }

    /* extend precomputation if necessary */
    if ((size_t)n > maxn) {
        size_t newsize = (size_t)n + 1;
        Catalan = (seq_t*)realloc(Catalan, newsize * sizeof(seq_t));
        Prefix  = (seq_t*)realloc(Prefix,  newsize * sizeof(seq_t));
        W       = (seq_t*)realloc(W,       newsize * sizeof(seq_t));
        A       = (seq_t*)realloc(A,       newsize * sizeof(seq_t));
        if (!Catalan || !Prefix || !W || !A) return (seq_t)0;   /* out of memory */

        for (size_t i = maxn + 1; i <= (size_t)n; ++i) {
            size_t im1 = i - 1;
            /* Catalan recurrence: C_i = C_{i-1} * 2*(2*(i-1)+1) / (i+1) */
            seq_t m     = (seq_t)im1;
            seq_t temp  = (seq_t)2 * m + (seq_t)1;       /* 2*m + 1   */
            temp = (seq_t)2 * temp;                       /* 2*(2*m+1) */
            Catalan[i] = Catalan[im1] * temp / (seq_t)(i + 1);

            /* Prefix sum of Catalan numbers */
            Prefix[i] = Prefix[im1] + Catalan[i];

            /* w_n = w_{n-1} + prefix[n] */
            W[i] = W[im1] + Prefix[i];

            /* a_n = w_n - 2*w_{n-1}  (for n>0) */
            if (i == 0)
                A[i] = (seq_t)1;
            else
                A[i] = W[i] - (seq_t)2 * W[im1];
        }
        maxn = (size_t)n;
    }
    return A[(size_t)n];
}
