/*
 * A364071  Triangle read by rows: T(n, k) = Sum_{d=0..n-k} binomial(n,
 *          d)*StirlingS2(n-d, k)*8^(n-d-k), with 0 <= k <= n.
 * https://oeis.org/A364071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   the sequence a(n) is the flattening of the triangle T(n,k) =
 *            Σ_{d=0..n-k} binomial(n,d) * StirlingS2(n-d,k) * 8^(n-d-k)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Pre–compute Stirling numbers of the second kind up to n = 200.
   The application only needs rows up to ~141 for n ≤ 10000. */
static seq_t stirling[201][201];
static int stirling_inited = 0;

/* Compute Stirling numbers up to the given level. */
static void compute_stirling(idx_t up_to)
{
    if (up_to > 200) up_to = 200;
    if (!stirling_inited) {
        for (idx_t i = 0; i <= 200; ++i)
            for (idx_t j = 0; j <= i; ++j)
                stirling[i][j] = 0;
        stirling[0][0] = 1;
        stirling_inited = 1;
    }
    for (idx_t i = 1; i <= up_to; ++i) {
        stirling[i][0] = 0;
        for (idx_t j = 1; j <= i; ++j) {
            seq_t prod = stirling[i-1][j] * (seq_t)j;
            stirling[i][j] = prod + stirling[i-1][j-1];
        }
    }
}

/* Flattened triangle */
seq_t a(idx_t n)
{
    /* find row r such that r*(r+3)/2 ≥ n */
    idx_t r = 0;
    while (r * (r + 3) / 2 < n)
        ++r;

    /* column index within the row */
    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    /* static array is large enough for the required n (≤ 10000) */
    compute_stirling(r);

    /* pre‑compute powers of 8 up to r */
    seq_t pow8[201];
    pow8[0] = 1;
    for (idx_t i = 1; i <= r; ++i)
        pow8[i] = pow8[i-1] * (seq_t)8;

    seq_t sum = 0;
    for (idx_t d = 0; d <= r - k; ++d) {
        idx_t nd  = r - d;       /* n-d */
        idx_t exp = nd - k;      /* r-d-k */

        seq_t term = binomial((idx_t)r, (idx_t)d);
        term = term * stirling[nd][k];
        term = term * pow8[exp];
        sum += term;
    }
    return sum;
}
