/*
 * A247255  Triangular array read by rows: T(n,k) is the number of weakly
 *          unimodal partitions of n in which the greatest part occurs exactly
 *          k times, n>=1, 1<=k<=n.
 * https://oeis.org/A247255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{g=1}^{⌊n/k⌋} PS[g][n-g*k] where PS[g][s] is the
 *            coefficient of x^s in ∏_{i=1}^{g-1} (1-x^i)^{-2}. T[n][k] is
 *            pre‑computed for 1 ≤ n,k ≤ 200 (sufficient for n ≤ 10000) and
 *            a(n) is retrieved by row‑major indexing
 * Range:     u8 n <= 55, u16 n <= 325, u32 n <= 2556, u64 n <= 8751,
 *            u128 n <= 8751, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_R 200          /* enough for n ≈ 10 000 (row ≤ 141) */

static idx_t row_of(idx_t n) {
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Static tables, computed once */
    static seq_t PS[MAX_R + 1][MAX_R + 1] = {0};
    static seq_t T[MAX_R + 1][MAX_R + 1]  = {0};
    static bool init = false;

    if (!init) {
        /* PS[1][0] = 1, all other entries already zero */
        PS[1][0] = (seq_t)1;

        /* Build PS[g][s] for g = 2 .. MAX_R */
        for (idx_t g = 2; g <= MAX_R; ++g) {
            idx_t step = g - 1;

            /* clear current row */
            for (idx_t s = 0; s <= MAX_R; ++s) PS[g][s] = (seq_t)0;

            /* convolution with (1 - x^step)^{-2} = Σ_{t≥0} (t+1)x^{step*t} */
            idx_t max_t = MAX_R / step;
            for (idx_t t = 0; t <= max_t; ++t) {
                seq_t weight = (seq_t)(t + 1);
                idx_t shift  = step * t;
                for (idx_t s = shift; s <= MAX_R; ++s) {
                    PS[g][s] += weight * PS[g - 1][s - shift];
                }
            }
        }

        /* Compute T[n][k] */
        for (idx_t nrow = 1; nrow <= MAX_R; ++nrow) {
            for (idx_t k = 1; k <= nrow; ++k) {
                seq_t sum = (seq_t)0;
                idx_t max_g = nrow / k;
                for (idx_t g = 1; g <= max_g; ++g) {
                    sum += PS[g][nrow - g * k];
                }
                T[nrow][k] = sum;
            }
        }

        init = true;
    }

    if (n == 0) return (seq_t)0;           /* not defined for n=0 */
    idx_t r = row_of(n);
    idx_t pre = (r - 1) * r / 2;
    idx_t k = n - pre;                     /* 1‑based column index */
    return T[r][k];
}
