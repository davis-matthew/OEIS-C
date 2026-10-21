/*
 * A251898  Number of (n+2)X(1+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum unequal to 4 or 5 and every diagonal and antidiagonal
 *          sum equal to 4 or 5.
 * https://oeis.org/A251898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 28, u64 n <= 68, u128 n <= 147,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A251898
   Compute number of (n+2)×3 arrays with entries 0..3 such that
   every 3×3 subblock has row and column sums not equal to 4 or 5,
   and both main diagonals sum to 4 or 5.
   The values are obtained by dynamic programming over rows.
   Complexity: O(n * 4096) time, O(1) memory (constant 64×64 states).
   This implementation works for any seq_t (including arbitrary-precision)
   and for any n ≥ 1 (returns 0 for n < 1). */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of helper functions supplied by the build framework */
#include "../utils/bitlen_seq_t.h"

/* Constants */
#define ROW_TOKENS 64          /* 4^3 possible 3‑cell rows (0..3) */
#define STATE_SIZE (ROW_TOKENS * ROW_TOKENS)  /* 4096 pair‑states */

/* Static tables and state */
static unsigned long long trans_masks[STATE_SIZE];
static int masks_initialized = 0;

static seq_t *dp_cur = NULL;
static seq_t *dp_next = NULL;
static int dp_initialized = 0;

/* Decode a 3‑cell row encoded as a base‑4 integer */
static void decode_row(int row, int *a, int *b, int *c)
{
    *a = (row >> 4) & 3;
    *b = (row >> 2) & 3;
    *c = row & 3;
}

/* Test whether three rows form a valid 3×3 subblock */
static int valid_subblock(int r1, int r2, int r3)
{
    int a1, b1, c1, a2, b2, c2, a3, b3, c3;
    decode_row(r1, &a1, &b1, &c1);
    decode_row(r2, &a2, &b2, &c2);
    decode_row(r3, &a3, &b3, &c3);

    /* Row sums must not be 4 or 5 */
    int s1 = a1 + b1 + c1;
    int s2 = a2 + b2 + c2;
    int s3 = a3 + b3 + c3;
    if (s1 == 4 || s1 == 5) return 0;
    if (s2 == 4 || s2 == 5) return 0;
    if (s3 == 4 || s3 == 5) return 0;

    /* Column sums must not be 4 or 5 */
    int col1 = a1 + a2 + a3;
    int col2 = b1 + b2 + b3;
    int col3 = c1 + c2 + c3;
    if (col1 == 4 || col1 == 5) return 0;
    if (col2 == 4 || col2 == 5) return 0;
    if (col3 == 4 || col3 == 5) return 0;

    /* Diagonals must equal 4 or 5 */
    int diag  = a1 + b2 + c3;
    int anti = a3 + b2 + c1;
    if (!( (diag  == 4) || (diag  == 5) )) return 0;
    if (!( (anti == 4) || (anti == 5) )) return 0;

    return 1;
}

/* Precompute the allowed third row for each pair (r1,r2) as a bit mask */
static void compute_masks(void)
{
    for (int r1 = 0; r1 < ROW_TOKENS; ++r1) {
        for (int r2 = 0; r2 < ROW_TOKENS; ++r2) {
            unsigned long long mask = 0;
            for (int r3 = 0; r3 < ROW_TOKENS; ++r3) {
                if (valid_subblock(r1, r2, r3))
                    mask |= 1ULL << r3;
            }
            trans_masks[r1 * ROW_TOKENS + r2] = mask;
        }
    }
}

/* Ensure dp buffers are allocated */
static void init_dp_buffers(void)
{
    if (!dp_cur) {
        dp_cur  = (seq_t*)calloc(STATE_SIZE, sizeof(seq_t));
        dp_next = (seq_t*)calloc(STATE_SIZE, sizeof(seq_t));
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Initialize resources once */
    if (!masks_initialized) {
        compute_masks();
        masks_initialized = 1;
    }
    init_dp_buffers();

    /* Number of rows in the array */
    size_t N = (size_t)n + 2;   /* N >= 3 for all n >= 1 */
    size_t remaining = N >= 2 ? N - 2 : 0;   /* number of rows after first two */

    /* Reset current DP buffer */
    for (size_t i = 0; i < STATE_SIZE; ++i)
        dp_cur[i] = 0;

    /* Initialize for the first two rows (no subblock constraint yet) */
    for (int r1 = 0; r1 < ROW_TOKENS; ++r1)
        for (int r2 = 0; r2 < ROW_TOKENS; ++r2)
            dp_cur[r1 * ROW_TOKENS + r2] = 1;

    /* Process remaining rows */
    for (size_t step = 0; step < remaining; ++step) {
        /* Zero dp_next buffer */
        for (size_t i = 0; i < STATE_SIZE; ++i)
            dp_next[i] = 0;

        for (int idx = 0; idx < STATE_SIZE; ++idx) {
            seq_t count = dp_cur[idx];
            if (count == 0) continue;
            int r1 = idx / ROW_TOKENS;
            int r2 = idx % ROW_TOKENS;
            unsigned long long m = trans_masks[idx];
            while (m) {
                int r3 = __builtin_ctzll(m);
                int next_idx = r2 * ROW_TOKENS + r3;
                dp_next[next_idx] += count;
                m &= m - 1;
            }
        }

        /* Swap buffers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum over all ending states to get the final answer */
    seq_t result = 0;
    for (size_t i = 0; i < STATE_SIZE; ++i)
        result += dp_cur[i];

    return result;
}
