/*
 * A291978  Triangle read by rows, T(n, k) = (-1)^(n-k)*n!*[t^k]([x^n]
 *          exp(x*t)/(1 + log(1+x))) for 0<=k<=n.
 * https://oeis.org/A291978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th entry of the triangle read row by row, where
 * the triangle T(r, c) is defined by
 *
 *          T(r, c) = binomial(r, r - c) *
 *                     Σ_{j=0}^{r-c} j! · |Stirling1(r-c, j)|
 *
 * for 0 ≤ c ≤ r.  The indices r and c satisfy
 *
 *          n = r·(r+1)/2 + c .
 *
 * The function below precomputes the unsigned Stirling numbers of the first
 * kind and the factorials up to row 200 (sufficient for n ≤ 10000) and
 * evaluates the required binomial and sum for the given index n.
 */

#include <stdbool.h>
#include "../utils/binomial.h"          /* binomial(idx_t, idx_t) -> seq_t */
#include "../utils/_floor_isqrt_u64.h"   /* _floor_isqrt_u64(idx_t) -> idx_t */

#define MAX_ROWS 200   /* enough for n up to about 10 000 */

static bool tables_initialized = false;
static seq_t S1[MAX_ROWS + 1][MAX_ROWS + 1];  /* unsigned Stirling numbers 1st kind */
static seq_t fact[MAX_ROWS + 1];              /* factorials */
static seq_t sumS[MAX_ROWS + 1];              /* Σ_{j=0}^{p} j! · abs(Stirling1(p,j)) */

/* Initialise the tables only once. */
static void init_tables(void)
{
    if (tables_initialized)
        return;

    /* 0! = 1 */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_ROWS; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* zero out the Stirling table */
    for (idx_t i = 0; i <= MAX_ROWS; ++i)
        for (idx_t j = 0; j <= MAX_ROWS; ++j)
            S1[i][j] = (seq_t)0;

    S1[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_ROWS; ++n)
    {
        for (idx_t k = 1; k <= n; ++k)
        {
            S1[n][k] = S1[n - 1][k - 1] +
                       ((seq_t)(n - 1) * S1[n - 1][k]);
        }
        /* remaining entries S1[n][k] for k > n are already 0 */
    }

    /* compute Σ_{j=0}^{p} j! * S1[p][j] for each p */
    for (idx_t p = 0; p <= MAX_ROWS; ++p)
    {
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j <= p; ++j)
            sum += fact[j] * S1[p][j];
        sumS[p] = sum;
    }

    tables_initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_tables();

    /* Find row r (here named m) and column c (named k) such that
       n = r·(r+1)/2 + c, 0 ≤ c ≤ r. */
    idx_t t = _floor_isqrt_u64((n << 3) + 1);          /* sqrt(8*n + 1) */
    idx_t r = (t - 1) >> 1;                           /* floor((t-1)/2) */
    idx_t c = n - (r * (r + 1)) / 2;                  /* column index */

    /* Let p = r - c; then T(r, c) = binomial(r, c) * sumS[p] */
    idx_t p = r - c;

    return binomial(r, c) * sumS[p];
}
