/*
 * A297751  Number of 3Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 1, 3 or 4 neighboring 1s.
 * https://oeis.org/A297751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 209 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over column patterns. */
/* Each 3×n binary matrix is processed column‑by‑column.  For every
   transition from the previous column `prev` to the current column
   `cur` and the new column `nxt`, the middle column `cur` must be
   valid, i.e. each 1 in `cur` must have 1, 3 or 4 horizontally or
   diagonally adjacent 1s (vertical neighbours are not counted).  The
   final column is validated against a trailing zero column. */

#include <stddef.h>
#include <stdbool.h>

/* Check if the middle column `mid` is valid when surrounded by
   `left` and `right`.  All columns are represented as 3‑bit
   patterns (bit 0 is the top row).  A 1 must have 1, 3 or 4
   horizontally or diagonally adjacent 1s.  (Vertical neighbours
   are not considered.) */
static bool valid_mid(int left, int mid, int right)
{
    int l0 = (left >> 0) & 1, l1 = (left >> 1) & 1, l2 = (left >> 2) & 1;
    int m0 = (mid >> 0) & 1,  m1 = (mid >> 1) & 1,  m2 = (mid >> 2) & 1;
    int r0 = (right >> 0) & 1, r1 = (right >> 1) & 1, r2 = (right >> 2) & 1;

    if (m0) {
        int neigh = l0 + r0 + l1 + r1;          /* top row */
        if (neigh != 1 && neigh != 3 && neigh != 4) return false;
    }
    if (m1) {
        int neigh = l1 + r1 + l0 + l2 + r0 + r2;  /* middle row */
        if (neigh != 1 && neigh != 3 && neigh != 4) return false;
    }
    if (m2) {
        int neigh = l2 + r2 + l1 + r1;          /* bottom row */
        if (neigh != 1 && neigh != 3 && neigh != 4) return false;
    }
    return true;
}

/* Pre‑computation of the validity table to speed up the DP. */
static bool valid_table[8][8][8];
static bool valid_table_inited = false;

static void init_valid_table(void)
{
    if (valid_table_inited) return;
    for (int l = 0; l < 8; ++l)
        for (int m = 0; m < 8; ++m)
            for (int r = 0; r < 8; ++r)
                valid_table[l][m][r] = valid_mid(l, m, r);
    valid_table_inited = true;
}

seq_t a(idx_t n)
{
    /* The offset is 1; values for n < 1 are irrelevant. */
    if (n == 0) return (seq_t)0;

    init_valid_table();

    /* dp[prev][cur] counts arrays ending with the two most recent
       columns `prev` and `cur`. */
    seq_t dp[8][8];
    seq_t next[8][8];
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            dp[i][j]  = (seq_t)0;
            next[i][j] = (seq_t)0;
        }

    /* Start with a fictitious left column of zeros. */
    for (int cur = 0; cur < 8; ++cur)
        dp[0][cur] = (seq_t)1;

    /* Process columns 2 … n.  After the loop, dp corresponds to
       the last two columns of the n‑column array. */
    for (idx_t step = 1; step < n; ++step) {
        for (int prev = 0; prev < 8; ++prev)
            for (int cur = 0; cur < 8; ++cur) {
                seq_t cnt = dp[prev][cur];
                if (cnt == (seq_t)0) continue;
                for (int nxt = 0; nxt < 8; ++nxt)
                    if (valid_table[prev][cur][nxt])
                        next[cur][nxt] += cnt;
            }
        /* Move next → dp and clear next. */
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j) {
                dp[i][j] = next[i][j];
                next[i][j] = (seq_t)0;
            }
    }

    /* Validate the last column against a trailing zero column. */
    seq_t total = (seq_t)0;
    for (int prev = 0; prev < 8; ++prev)
        for (int cur = 0; cur < 8; ++cur)
            if (valid_table[prev][cur][0])
                total += dp[prev][cur];

    return total;
}
