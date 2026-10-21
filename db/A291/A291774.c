/*
 * A291774  Triangle read by rows: chromatic invariant T(n,m) of the complete
 *          bipartite graph K_{m,n}.
 * https://oeis.org/A291774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the triangle T(m,n) read by rows with m = column
 *            index (1‑based), n = row index (1‑based). The triangle T
 *            satisfies T(m,n) = Σ_{k=0..m-1} k! · (−1)^{k+m} · (k+1)^n ·
 *            S(m,k+2) where S(m,k) are Stirling numbers of the second kind.
 *            For the initial term T(1,1)=1. The code implements t...
 * Range:     u8 n <= 13, u16 n <= 20, u32 n <= 43, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

#define MAXM 200        /* enough for n up to ~10000 */

/* Stirling numbers of the second kind, S[i][j] = S(i,j) */
static seq_t S[MAXM + 1][MAXM + 3];
static bool stirling_prepared = false;

static void prepare_stirling(void)
{
    if (stirling_prepared) return;
    /* Initialize all to 0 (static array already zero‑initialised) */
    S[0][0] = 1;
    for (idx_t i = 1; i <= MAXM; ++i) {
        S[i][0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            S[i][j] = S[i - 1][j - 1] + ((seq_t)j) * S[i - 1][j];
        }
        /* S[i][i+1] remains 0 */
    }
    stirling_prepared = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined, but safe */

    /* find row R such that R*(R+1)/2 >= n */
    idx_t R = 1;
    while (((seq_t)R * (R + 1) / 2) < n) ++R;
    /* column index C */
    idx_t prev_rows = (R - 1) * R / 2;
    idx_t C = n - prev_rows;    /* 1‑based */

    /* special case T(1,1) */
    if (R == 1 && C == 1) return 1;

    prepare_stirling();

    idx_t m = C;             /* smaller index */
    idx_t large = R;         /* larger index */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t k = 0; k < m; ++k) {
        seq_t fact_k = factorial(k);
        seq_t pow_val = big_pow((seq_t)(k + 1), (seq_t)large);
        seq_t term = fact_k * pow_val;
        term *= S[m][k + 2];

        if (((k + m) & 1) == 0) {
            sum_pos += term;
        } else {
            sum_neg += term;
        }
    }

    /* final result (sum_pos ≥ sum_neg for this sequence) */
    return sum_pos - sum_neg;
}
