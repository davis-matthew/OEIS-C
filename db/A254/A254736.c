/*
 * A254736  Number of (n+2) X (1+2) 0..1 arrays with every 3 X 3 subblock sum
 *          of the two medians of the central row and column minus the sum of
 *          the minimums of the diagonal and antidiagonal nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A254736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 27, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254736
 *  Number of (n+2)×3 binary arrays whose every 3×3 subblock satisfies
 *  the median‑sum minus min‑diagonal rule, and the resulting sequence
 *  is nondecreasing.  The function computes the term by dynamic
 *  programming over the last two rows and the previous block value.
 *  The recurrence has no closed form known; the DP runs in O((n)·64·5·8)
 *  time and O(64·5) memory.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Precomputed table of the block value e = median(row)+median(col)
 *        - min(diagonal) - min(antidiagonal) for all 8×8×8 triples.
 *  e ranges from –2 to 2, stored as int8_t. */
static int8_t E_tbl[8][8][8];
static bool E_tbl_ready = false;

static void init_E_tbl(void)
{
    if (E_tbl_ready) return;
    for (int a = 0; a < 8; ++a) {
        for (int b = 0; b < 8; ++b) {
            for (int c = 0; c < 8; ++c) {
                int tr  = ((b      ) & 1) + (((b >> 1)   ) & 1) + (((b >> 2)   ) & 1);
                int median_row = (tr >= 2) ? 1 : 0;

                int tc  = (((a >> 1)   ) & 1) + (((b >> 1)   ) & 1) + (((c >> 1)   ) & 1);
                int median_col = (tc >= 2) ? 1 : 0;

                int min_diag  = ((a & 1)          ) & (((b >> 1)   ) & 1) & (((c >> 2)   ) & 1);
                int min_anti  = (((a >> 2)   ) & 1) & (((b >> 1)   ) & 1) & ((c & 1)     );

                int e = median_row + median_col - min_diag - min_anti;
                E_tbl[a][b][c] = (int8_t)e;   /* –2 … 2 */
            }
        }
    }
    E_tbl_ready = true;
}

static const int STATE_ROWS = 64;   /* 3‑bit pair of rows: r2<<3 | r3 */
static const int E_VALUES   = 5;    /* e + 2 maps –2…2 to 0…4 */
static const size_t DP_SIZE = STATE_ROWS * E_VALUES;

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined below offset */

    init_E_tbl();

    idx_t m = n + 2;                 /* total number of rows */

    seq_t dp[DP_SIZE];
    seq_t next[DP_SIZE];

    /* zero initialise DP arrays */
    for (size_t i = 0; i < DP_SIZE; ++i) dp[i] = (seq_t)0;
    for (size_t i = 0; i < DP_SIZE; ++i) next[i] = (seq_t)0;

    /* Initial state after first 3 rows (no monotonicity yet) */
    for (int r1 = 0; r1 < 8; ++r1) {
        for (int r2 = 0; r2 < 8; ++r2) {
            for (int r3 = 0; r3 < 8; ++r3) {
                int e      = (int)E_tbl[r1][r2][r3];
                int e_idx  = e + 2;              /* 0 … 4 */
                int state  = (r2 << 3) | r3;     /* last two rows */
                dp[state * E_VALUES + e_idx] += (seq_t)1;
            }
        }
    }

    /* Process rows 4 … m (if any). */
    for (idx_t row = 4; row <= m; ++row) {
        /* zero next array */
        for (size_t i = 0; i < DP_SIZE; ++i) next[i] = (seq_t)0;

        for (int s = 0; s < STATE_ROWS; ++s) {
            int prev1 = s >> 3;          /* second‑to‑last row   */
            int prev2 = s & 7;           /* last row             */

            for (int e_idx = 0; e_idx < E_VALUES; ++e_idx) {
                seq_t val = dp[s * E_VALUES + e_idx];
                if (val == (seq_t)0) continue;

                int prev_E = e_idx - 2;  /* actual e ∈ –2 … 2 */

                for (int cur = 0; cur < 8; ++cur) {
                    int new_E = (int)E_tbl[prev1][prev2][cur];
                    if (new_E < prev_E) continue;         /* nondecreasing */

                    int new_state = (prev2 << 3) | cur;
                    int new_e_idx = new_E + 2;
                    next[new_state * E_VALUES + new_e_idx] += val;
                }
            }
        }

        /* copy next into dp for next iteration */
        for (size_t i = 0; i < DP_SIZE; ++i) dp[i] = next[i];
    }

    /* Sum all finished states */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < DP_SIZE; ++i) result += dp[i];
    return result;
}
