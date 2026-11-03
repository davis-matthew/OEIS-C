/*
 * A397139  Number of Hotelling protocols in a two-person game where each
 *          player has n distinguishable moves (e.g. "announce a price",
 *          "announce a location", etc.).
 * https://oeis.org/A397139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n, j=1..n} i!*j!*S(n,i)*S(n,j)*t(i,j), where
 *            S(n,k) are Stirling numbers of the second kind and t(i,j) are
 *            the coefficients of (1+x+y+xy)/(1-2xy-x^2y-xy^2-x^2y^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Convert index to a platform‑independent integer first */
    idx_t N = n;                     /* N ≥ 1 */

    /* ---------- factorials ---------- */
    seq_t *fact = mem_new(seq_t, (size_t)N + 1);
    fact[0] = 1;
    for (size_t i = 1; i <= (size_t)N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* ---------- Stirling numbers S(n,k) ---------- */
    seq_t *prev = mem_new(seq_t, (size_t)N + 1);
    seq_t *curr = mem_new(seq_t, (size_t)N + 1);
    prev[0] = 1;                     /* S(0,0)=1 */

    for (size_t m = 1; m <= (size_t)N; ++m) {
        curr[0] = 0;
        for (size_t k = 1; k <= m; ++k)
            curr[k] = prev[k - 1] + (seq_t)k * prev[k];
        /* swap prev and curr */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }
    /* now prev[k] = S(N,k) for all k */

    /* ---------- A(k) = k! * S(N,k) ---------- */
    seq_t *A = mem_new(seq_t, (size_t)N + 1);
    for (size_t k = 1; k <= (size_t)N; ++k)
        A[k] = fact[k] * prev[k];

    /* ---------- t(i,j) rows ---------- */
    seq_t *row_prev2 = mem_new(seq_t, (size_t)N + 1);
    seq_t *row_prev  = mem_new(seq_t, (size_t)N + 1);
    seq_t *row_cur   = mem_new(seq_t, (size_t)N + 1);

    /* initialize i = 0 row: t(0,0)=t(0,1)=t(1,0)=t(1,1)=1, others 0 */
    for (size_t j = 0; j <= (size_t)N; ++j)
        row_prev[j] = (j == 0 || j == 1) ? 1 : 0;

    seq_t ans = 0;

    for (size_t i = 1; i <= (size_t)N; ++i) {
        /* compute row for current i */
        for (size_t j = 0; j <= (size_t)N; ++j) {
            seq_t rhs = ((i == 0 && j == 0) ||
                         (i == 1 && j == 0) ||
                         (i == 0 && j == 1) ||
                         (i == 1 && j == 1)) ? 1 : 0;

            seq_t val = rhs;
            if (i >= 1) {
                if (j >= 1) {
                    val += row_prev[j - 1] * 2;
                    val += row_prev2[j - 1];
                }
                if (j >= 2) {
                    val += row_prev[j - 2];
                    val += row_prev2[j - 2];
                }
            }
            row_cur[j] = val;
        }

        /* accumulate contribution for this i */
        seq_t inner_sum = 0;
        for (size_t j = 1; j <= (size_t)N; ++j)
            inner_sum += row_cur[j] * A[j];

        ans += A[i] * inner_sum;

        /* rotate rows: row_prev2 ← row_prev, row_prev ← row_cur */
        seq_t *t = row_prev2;
        row_prev2 = row_prev;
        row_prev  = row_cur;
        row_cur   = t;          /* reuse the old buffer */
    }

    /* free memory */
    mem_free(fact);
    mem_free(prev);
    mem_free(curr);
    mem_free(A);
    mem_free(row_prev2);
    mem_free(row_prev);
    mem_free(row_cur);

    return ans;
}
