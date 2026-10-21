/*
 * A289955  Number of permutations of [n] having exactly seven nontrivial
 *          cycles.
 * https://oeis.org/A289955
 *
 * Offset:    14    (a(14) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^{n-14} binom(n, j) * c(n-j, 7) where c(m, k)
 *            counts permutations of m elements having exactly k cycles all of
 *            length ≥ 2. The numbers c(m, k) are obtained from the recurrence
 *            c(m, k) = (m-1) * c(m-1, k) + (m-1) * c(m-2, k-1), with c(0,0)=1
 *            and c(m,0)=0 for m>0. This recurrence foll...
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_N 10000  /* maximum n required by the problem statement */

static seq_t c[8][MAX_N + 1];          /* c[k][m] → c(m, k)  */
static bool table_initialized = false;

/* pre‑compute the table of c(m,k) for 0 ≤ m ≤ MAX_N, 0 ≤ k ≤ 7 */
static void init_table(void) {
    int k, m;
    /* initialise all entries to zero */
    for (k = 0; k <= 7; ++k)
        for (m = 0; m <= MAX_N; ++m)
            c[k][m] = 0;

    c[0][0] = 1;                      /* base case */

    for (m = 1; m <= MAX_N; ++m) {
        c[0][m] = 0;                  /* no permutation of m>0 has 0 cycles */
        for (k = 1; k <= 7; ++k) {
            seq_t term1 = (seq_t)(m - 1) * c[k][m - 1];
            seq_t term2 = 0;
            if (m >= 2)
                term2 = (seq_t)(m - 1) * c[k - 1][m - 2];
            c[k][m] = term1 + term2;
        }
    }
    table_initialized = true;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (!table_initialized)
        init_table();

    if (n < 14)                 /* offset is 14, values before are undefined */
        return 0;

    seq_t ans   = 0;
    seq_t binom = 1;            /* binom(n,0) */
    idx_t max_fixed = n - 14;   /* maximum number of fixed points */

    for (idx_t j = 0; j <= max_fixed; ++j) {
        ans = ans + binom * c[7][n - j];
        if (j < max_fixed)
            binom = (binom * (seq_t)(n - j)) / (seq_t)(j + 1);
    }
    return ans;
}
