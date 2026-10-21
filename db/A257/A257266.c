/*
 * A257266  Number of (n+2)X(1+2) 0..2 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum or central row sum equal to the
 *          central column sum.
 * https://oeis.org/A257266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 17, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A257266:
   Count of (n+2)×3 arrays with entries in {0,1,2}
   such that for every 3×3 subblock:
     - diagonal sum ≥ antidiagonal sum
     - central row sum ≠ central column sum
   The constraints depend only on consecutive triples of rows,
   so we use dynamic programming over pairs of preceding rows.  */

#include <stdint.h>
#include <string.h>

/* Helper to precompute the 27×27 transition masks. */
static uint32_t _row_dig[27][3];
static uint32_t _allowed[27][27];
static int _allowed_computed = 0;

static void _compute_allowed(void)
{
    if (_allowed_computed) return;
    /* Encode each row as 3 digits 0..2. */
    for (int r = 0; r < 27; ++r) {
        int a = r / 9;
        int b = (r % 9) / 3;
        int c = r % 3;
        _row_dig[r][0] = a;
        _row_dig[r][1] = b;
        _row_dig[r][2] = c;
    }

    /* For each pair (r1,r2) compute mask of allowed r3. */
    for (int r1 = 0; r1 < 27; ++r1) {
        for (int r2 = 0; r2 < 27; ++r2) {
            uint32_t mask = 0;
            for (int r3 = 0; r3 < 27; ++r3) {
                int a1 = _row_dig[r1][0], b1 = _row_dig[r1][1], c1 = _row_dig[r1][2];
                int a2 = _row_dig[r2][0], b2 = _row_dig[r2][1], c2 = _row_dig[r2][2];
                int a3 = _row_dig[r3][0], b3 = _row_dig[r3][1], c3 = _row_dig[r3][2];

                int diag_sum      = a1 + b2 + c3;
                int anti_diag_sum = c1 + b2 + a3;
                if (diag_sum < anti_diag_sum) continue;

                int central_row_sum = a2 + b2 + c2;
                int central_col_sum = b1 + b2 + b3;
                if (central_row_sum == central_col_sum) continue;

                mask |= (1u << r3);
            }
            _allowed[r1][r2] = mask;
        }
    }
    _allowed_computed = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n>=1 by definition.  m = number of rows = n+2. */
    idx_t m = n + 2;

    _compute_allowed();

    static seq_t dp[27 * 27];
    static seq_t next_dp[27 * 27];

    /* Initialise dp: one way for each pair of first two rows. */
    for (int i = 0; i < 27 * 27; ++i) dp[i] = 0;
    for (int r1 = 0; r1 < 27; ++r1)
        for (int r2 = 0; r2 < 27; ++r2)
            dp[r1 * 27 + r2] = 1;

    idx_t steps = m - 2;   /* number of transitions to add rows. */
    for (idx_t step = 0; step < steps; ++step) {
        for (int i = 0; i < 27 * 27; ++i) next_dp[i] = 0;

        for (int r1 = 0; r1 < 27; ++r1) {
            for (int r2 = 0; r2 < 27; ++r2) {
                seq_t cnt = dp[r1 * 27 + r2];
                if (cnt == 0) continue;
                uint32_t mask = _allowed[r1][r2];
                while (mask) {
                    unsigned int lb = mask & -mask;
                    int r3 = __builtin_ctz(mask);
                    int idx = r2 * 27 + r3;
                    next_dp[idx] += cnt;
                    mask ^= lb;
                }
            }
        }
        /* swap dp and next_dp */
        for (int i = 0; i < 27 * 27; ++i) dp[i] = next_dp[i];
    }

    /* Sum all counts to get the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < 27 * 27; ++i) total += dp[i];
    return total;
}
