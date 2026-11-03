/*
 * A383900  Square array A(n,k), n>=0, k>=0, read by antidiagonals downwards,
 *          where column k is the expansion of Product_{j=0..k} (1 + j*x)/(1 -
 *          j*x).
 * https://oeis.org/A383900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Sum_{j=0..k} |Stirling1(k+1,j+1)| * Stirling2(j+n,k)
 *            where (n,k) are obtained by reading the square array A(n,k)
 *            along antidiagonals downwards
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t sum_stirling(idx_t idx)
{
    /* Determine diagonal (s) and offset within that diagonal */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= idx)
        ++s;
    idx_t offset = idx - s * (s + 1) / 2;
    idx_t r = offset;           /* row index   */
    idx_t k = s - offset;       /* column index */

    /* The largest Stirling argument needed: r + k */
    idx_t nmax = r + k;

    /* Allocate 2‑D tables for unsigned Stirling numbers of first and
       second kind.  Rows are indexed by n (0 .. nmax+1), columns by
       k (0 .. n).  Memory is freed after the computation. */
    seq_t **S1 = (seq_t **)malloc((nmax + 2) * sizeof(seq_t *));
    seq_t **S2 = (seq_t **)malloc((nmax + 2) * sizeof(seq_t *));
    if (!S1 || !S2)
        return 0;   /* allocation failure – unlikely in tests */

    for (idx_t i = 0; i <= nmax + 1; ++i) {
        S1[i] = mem_new(seq_t, i + 1);
        S2[i] = mem_new(seq_t, i + 1);
        if (!S1[i] || !S2[i]) {
            for (idx_t j = 0; j <= i; ++j) {
                mem_free(S1[j]); mem_free(S2[j]);
            }
            free(S1); free(S2);
            return 0;
        }
    }

    /* Base cases */
    S1[0][0] = (seq_t)1;
    S2[0][0] = (seq_t)1;

    /* Build tables up to nmax+1 */
    for (idx_t n = 1; n <= nmax + 1; ++n) {
        for (idx_t m = 0; m <= n; ++m) {
            /* unsigned Stirling numbers of the first kind |
               s1(n, m) = s1(n-1, m-1) + (n-1)*s1(n-1, m) |
               treat out-of-range indices as zero */
            seq_t a = 0;
            if (m >= 1 && m - 1 <= (idx_t)(n - 1))
                a += S1[n - 1][m - 1];
            if (m <= (idx_t)(n - 1))
                a += (seq_t)(n - 1) * S1[n - 1][m];
            S1[n][m] = a;

            /* Stirling numbers of the second kind */
            seq_t b = 0;
            if (m >= 1 && m - 1 <= (idx_t)(n - 1))
                b += S2[n - 1][m - 1];
            if (m <= (idx_t)(n - 1))
                b += (seq_t)m * S2[n - 1][m];
            S2[n][m] = b;
        }
    }

    /* Compute the desired entry A(r,k) */
    seq_t result = (seq_t)0;
    for (idx_t j = 0; j <= k; ++j) {
        /* term = |s1(k+1, j+1)| * s2(j+r, k) , second factor zero if
           j+r < k */
        if (j + r >= k) {
            seq_t term = S1[k + 1][j + 1] * S2[j + r][k];
            result += term;
        }
    }

    /* Free memory */
    for (idx_t i = 0; i <= nmax + 1; ++i) {
        mem_free(S1[i]);
        mem_free(S2[i]);
    }
    free(S1);
    free(S2);

    return result;
}

seq_t a(idx_t n)
{
    return sum_stirling(n);
}
