/*
 * A394116  G.f. A(x) satisfies
 *          A(x)^5-(2x+3)*A(x)^4+(7x+1)*A(x)^3-(4x^2+3x)*A(x)^2+3x^2*A(x)-x^3=0.
 * https://oeis.org/A394116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes OEIS sequence A394116.
   The combinatorial specification gives the following
   functional equations for the ordinary generating
   functions of red, blue and green vertices:

      R(x) = x / (1 - G(x))
      G(x) = x / (1 - (B(x) + G(x)))
      B(x) = x / (1 - (R(x) + B(x)))

   From them we obtain the recurrences

      B[n] = δ_{n,1} + Σ_{i=1}^{n-1} R[i]*B[n-i] + Σ_{i=1}^{n-1} B[i]*B[n-i]
      G[n] = δ_{n,1} + Σ_{i=1}^{n-1} B[i]*G[n-i] + Σ_{i=1}^{n-1} G[i]*G[n-i]
      R[n] = δ_{n,1} + Σ_{i=1}^{n-1} R[i]*G[n-i]

   where δ_{n,1} is 1 when n==1 and 0 otherwise.
   The desired term a(n) is R[n] (with a(0)=0).  The computation
   proceeds incrementally and memoises all previously
   computed values, so calls can be made in any order. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Base case : a(0) = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Static storage */
    static seq_t *R = NULL, *B = NULL, *G = NULL;
    static size_t cap = 0;        /* allocated capacity (>= max_n+1) */
    static idx_t maxn = 0;        /* largest computed index */

    /* Grow arrays if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = n + 1;
        if (new_cap < 256)
            new_cap = 256;

        R = seq_realloc(R, cap, new_cap);
        B = seq_realloc(B, cap, new_cap);
        G = seq_realloc(G, cap, new_cap);
        /* Initialise any newly allocated entries to zero */
        for (size_t i = cap; i < new_cap; ++i) {
            R[i] = (seq_t)0;
            B[i] = (seq_t)0;
            G[i] = (seq_t)0;
        }
        cap = new_cap;
    }

    /* Compute terms from maxn+1 up to n */
    for (idx_t m = maxn + 1; m <= n; ++m) {
        /* Compute B[m] */
        seq_t s = (seq_t)0;
        for (idx_t i = 1; i < m; ++i)
            s += R[i] * B[m - i];
        for (idx_t i = 1; i < m; ++i)
            s += B[i] * B[m - i];
        B[m] = (m == 1) ? (seq_t)1 : s;

        /* Compute G[m] */
        s = (seq_t)0;
        for (idx_t i = 1; i < m; ++i)
            s += B[i] * G[m - i];
        for (idx_t i = 1; i < m; ++i)
            s += G[i] * G[m - i];
        G[m] = (m == 1) ? (seq_t)1 : s;

        /* Compute R[m] */
        s = (seq_t)0;
        for (idx_t i = 1; i < m; ++i)
            s += R[i] * G[m - i];
        R[m] = (m == 1) ? (seq_t)1 : s;
    }

    maxn = n;
    return R[n];
}
