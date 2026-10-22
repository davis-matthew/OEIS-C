/*
 * A338980  Number of oriented colorings of the 120 dodecahedral facets of the
 *          4-D 120-cell (or 120 vertices of the 4-D 600-cell) using exactly n
 *          colors.
 * https://oeis.org/A338980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini
 * Formula:   a(n) = n! * (
 *            Σ_{j∈{4,6,8,10,12,16,20,22,24,30,32,40,44,60,62,120}} \ M_j *
 *            S₂(j,n) ) / 7200, where M_j are the integer multipliers of each
 *            term (2/15→960, 1/5→1440, 2/15→960, 1/6→1200, 7/150→336,
 *            1/25→288, 1/180→40, 1/18→400, 7/150→336, 1/120→60, 1/25→288,
 *            1/180→40, 1/18→400, 1/7200→1, 1/16→450,...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* may be needed for size_t */

static const idx_t j_vals[] = {4,6,8,10,12,16,20,22,24,30,32,40,44,60,62,120};
static const seq_t M_vals[] = {960,1440,960,1200,336,288,40,400,336,60,288,40,400,1,450,1};

/* Precomputed factorials up to 120 and Stirling numbers S₂(n,k). */
static seq_t fac[121];
static seq_t S2[121][121];
static int init_flag = 0;

static void init_tables(void)
{
    if (init_flag) return;
    fac[0] = 1;
    for (idx_t i = 1; i <= 120; ++i) fac[i] = fac[i-1] * (seq_t)i;

    for (idx_t n = 0; n <= 120; ++n)
        for (idx_t k = 0; k <= 120; ++k)
            S2[n][k] = 0;
    S2[0][0] = 1;
    for (idx_t n = 1; n <= 120; ++n)
        for (idx_t k = 1; k <= n; ++k)
            S2[n][k] = (seq_t)k * S2[n-1][k] + S2[n-1][k-1];

    init_flag = 1;
}

seq_t a(idx_t n)
{
    init_tables();
    if (n > 120 || n == 0) return 0;

    seq_t sum = 0;
    const size_t J = sizeof(j_vals)/sizeof(j_vals[0]);
    for (size_t t = 0; t < J; ++t) {
        idx_t j = j_vals[t];
        if (n <= j) sum += M_vals[t] * S2[j][n];
    }

    seq_t result = fac[n] * sum;
    result /= 7200;
    return result;
}
