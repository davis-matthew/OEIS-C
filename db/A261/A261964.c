/*
 * A261964  Chocolate numbers read as a triangle across rows: T(n,k), n >= 1,
 *          1 <= k <= n.
 * https://oeis.org/A261964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained from the chocolate‑bar triangle. The triangle
 *            entry T(r,k) = A(r+1-k,k); A(m,n) equals 1 when max(m,n)<2. For
 *            all other m,n the recurrence from OEIS A261964 is used: A(m,n) =
 *            Σ_{i=1}^{m-1} binom(m*n-2, i*n-1) * A(i,n) * A(m-i,n) +
 *            Σ_{i=1}^{n-1} binom(m*n-2, i*m-1) * A(m,i) * A(m,...
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 23, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factorial.h"   /* factorial(idx_t) -> seq_t */
#include "../utils/binom.h"       /* binom(idx_t a, idx_t b) -> seq_t */

static seq_t *A = NULL;      /* flat 2‑D table: A[m*(R+1)+n]  (1‑based)      */
static idx_t  maxR = 0;      /* current maximum row/column computed          */

/* Allocate and compute the table up to row/column R (R >= 1).         */
static void compute_table(idx_t R)
{
    idx_t stride = R + 1;                     /* array dimension           */
    size_t sz = (size_t)stride * stride * sizeof(seq_t);
    seq_t *newA = (seq_t *)malloc(sz);        /* cast for C++ compatibility */
    if (!newA) return;                        /* allocation failure         */

    /* Base case A(1,j) and A(j,1) = (j-1)!                               */
    for (idx_t j = 1; j <= R; ++j) {
        seq_t f = factorial(j - 1);           /* (j-1)!                     */
        newA[1 * stride + j] = f;
        newA[j * stride + 1] = f;
    }

    /* Compute all other entries (m>1 and n>1).                         */
    for (idx_t m = 2; m <= R; ++m) {
        for (idx_t n = 2; n <= R; ++n) {
            seq_t sum = (seq_t)0;
            idx_t total = m * n - 2;            /* m*n-2                       */

            /* First sum over i = 1 .. m-1                                 */
            for (idx_t i = 1; i < m; ++i) {
                seq_t coeff = binom(total, i * n - 1);
                seq_t prod  = newA[i * stride + n];
                prod *= newA[(m - i) * stride + n];
                prod *= coeff;
                sum += prod;
            }

            /* Second sum over j = 1 .. n-1                                 */
            for (idx_t j = 1; j < n; ++j) {
                seq_t coeff = binom(total, j * m - 1);
                seq_t prod  = newA[m * stride + j];
                prod *= newA[m * stride + (n - j)];
                prod *= coeff;
                sum += prod;
            }

            newA[m * stride + n] = sum;
        }
    }

    free(A);            /* discard old table if any                         */
    A     = newA;
    maxR  = R;
}

/* Return the n‑th term of the A261964 sequence (1‑based).             */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* behaviour for n<1 undefined */

    /* Find the smallest r such that r(r+1)/2 >= n.                     */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Ensure the table is large enough.                                 */
    if (r > maxR) compute_table(r);

    /* Position within row r (k = 1 .. r).                               */
    idx_t k = n - r * (r - 1) / 2;
    idx_t m = r + 1 - k;                        /* m = r+1-k,  n_col = k */

    idx_t stride = maxR + 1;
    return A[m * stride + k];
}
