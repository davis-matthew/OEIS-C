/*
 * A282575  Triangular array read by rows. T(n,k) is the number of minimal
 *          covers of an n-set with exactly k points that are in more than one
 *          set of the cover, n>=0, 0<=k<=max(0,n-2).
 * https://oeis.org/A282575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = binomial(n,k) * sum_{m=1}^{n-k} S(n-k,m) * (2^m - m -
 *            1)^k where S(a,b) are Stirling numbers of the second kind. The
 *            sequence a(n) is the triangular array T(n,k) read by rows. Row 0
 *            has length 1, row 1 has length 1, row 2 has length 1, and for
 *            n>=3 the length is n-1
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 49, u64 n <= 98, u128 n <= 201,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/pow2.h"
#include <stdbool.h>

#define MAX_PRECOMP 300

static bool precomputed = false;
static seq_t S[MAX_PRECOMP + 1][MAX_PRECOMP + 1];
static seq_t pow2_val[MAX_PRECOMP + 1];

static void init_precomp(void)
{
    /* precompute powers of two */
    pow2_val[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_PRECOMP; ++i) {
        pow2_val[i] = pow2((idx_t)i);
    }
    /* Stirling numbers of 2nd kind */
    for (idx_t i = 0; i <= MAX_PRECOMP; ++i) {
        for (idx_t j = 0; j <= MAX_PRECOMP; ++j) {
            S[i][j] = 0;
        }
    }
    S[0][0] = (seq_t)1;
    for (idx_t n = 1; n <= MAX_PRECOMP; ++n) {
        for (idx_t k = 1; k <= n; ++k) {
            seq_t term1 = S[n-1][k] * (seq_t)k;
            seq_t term2 = S[n-1][k-1];
            S[n][k] = term1 + term2;
        }
    }
    precomputed = true;
}

/* Convert flattened index to (row,k) */
static void index_to_row_col(idx_t idx, idx_t *row, idx_t *k)
{
    idx_t r = 0;
    idx_t cum = 0;
    while (1) {
        idx_t len = (r <= 2) ? 1 : r - 1;
        if (cum + len > idx) break;
        cum += len;
        ++r;
    }
    *row = r;
    *k   = idx - cum;
}

seq_t a(idx_t n)
{
    if (!precomputed) init_precomp();

    idx_t row, k;
    index_to_row_col(n, &row, &k);

    if (row == 0) {                 /* only term T(0,0) = 1 */
        return (seq_t)1;
    }

    /* Binomial coefficient C(row,k) */
    seq_t binom_val = binomial(row, k);

    /* Sum over m */
    seq_t sum = 0;
    idx_t nk = row - k;              /* number of unique elements */
    for (idx_t m = 1; m <= nk; ++m) {
        seq_t stir = S[nk][m];       /* S(n-k, m) */

        /* (2^m - m - 1) */
        seq_t baseMinus = pow2_val[m];
        baseMinus -= (seq_t)m;
        baseMinus -= (seq_t)1;

        seq_t powExp;
        if (k == 0) {
            powExp = (seq_t)1;       /* any number^0 = 1 */
        } else {
            powExp = big_pow(baseMinus, (seq_t)k);
        }

        seq_t term = stir * powExp;
        sum += term;
    }

    return binom_val * sum;
}
