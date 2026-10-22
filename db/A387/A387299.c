/*
 * A387299  Array read by downward antidiagonals: T(n,k) is the number of
 *          partitions of [n], n >= 1, k >= 1, into sets labeled with positive
 *          integers, such that the labels sum to k.
 * https://oeis.org/A387299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term read by downward antidiagonals of the
 *            array T(n,k) where T(n,k)=sum_{j=1..k} C(k-1,j-1)*Stirling2(n,j)
 * Range:     u8 n <= 41, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_STIRLING 200   /* enough for all n,k needed up to n=10 000 */

static seq_t Stirling[MAX_STIRLING+1][MAX_STIRLING+1] = {{0}};
static bool StirlingInit = false;

/* compute Stirling numbers of the second kind up to MAX_STIRLING */
static void init_stirling(void)
{
    if (StirlingInit) return;
    Stirling[0][0] = 1;
    for (idx_t n = 1; n <= MAX_STIRLING; ++n) {
        Stirling[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = Stirling[n-1][k-1];
            seq_t term2 = ((seq_t)k) * Stirling[n-1][k];
            Stirling[n][k] = term1 + term2;
        }
    }
    StirlingInit = true;
}

/* binomial coefficient C(n,k) computed multiplicatively.
   Assumes 0 <= k <= n <= MAX_STIRLING. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* compute T(n,k) according to the definition.  n,k >= 1. */
static seq_t T_value(idx_t n, idx_t k)
{
    init_stirling();
    seq_t sum = 0;
    for (idx_t j = 1; j <= k; ++j) {
        seq_t c = binom_small(k-1, j-1);
        seq_t term = c * Stirling[n][j];
        sum += term;
    }
    return sum;
}

/* convert a linear index m (>=1) to coordinates (n,k) of the downward
   antidiagonal reading, and return the corresponding term. */
seq_t a(idx_t m)
{
    /* find minimal d such that d*(d+1)/2 >= m */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < m) ++d;

    idx_t prev = ((d - 1) * d) / 2;           /* elements before diag d */
    idx_t offset = m - prev - 1;             /* zero‑based position in diag d */
    idx_t n_coord = offset + 1;
    idx_t k_coord = d - offset;

    return T_value(n_coord, k_coord);
}
