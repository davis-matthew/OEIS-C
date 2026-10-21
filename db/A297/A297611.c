/*
 * A297611  Number of 5Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays in which every 1 has 0, 1 or
 *            3 neighbouring 1's (horizontally, diagonally or antidiagonally).
 *            Computed by a transfer‑matrix dynamic programme
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Small helper: true iff a transition from left, middle to right
   satisfies the neighbour‑count condition for all 1's in the middle. */
static inline bool valid_transition(int left_buf, int mid, int right_buf)
{
    for (int r = 0; r < 5; ++r) {
        if (mid & (1 << r)) {
            int cnt = 0;
            /* neighbours on the left side */
            if (r > 0 && (left_buf & (1 << (r-1)))) ++cnt;
            if (           (left_buf & (1 << r)))          ++cnt;
            if (r < 4 && (left_buf & (1 << (r+1)))) ++cnt;
            /* neighbours on the right side */
            if (r > 0 && (right_buf & (1 << (r-1)))) ++cnt;
            if (           (right_buf & (1 << r)))          ++cnt;
            if (r < 4 && (right_buf & (1 << (r+1)))) ++cnt;
            if (cnt != 0 && cnt != 1 && cnt != 3) return false;
        }
    }
    return true;
}

/* Pre‑computed transition lists: for each (left,mid) pair we store all
   possible right masks and the number of successors.  Indices run 0..31. */
static uint8_t trans_next[32][32][32];
static uint8_t trans_nnext[32][32];
static bool trans_ready = false;

/* Build the transition tables once. */
static void build_trans(void)
{
    if (trans_ready) return;
    for (int left = 0; left < 32; ++left) {
        for (int mid = 0; mid < 32; ++mid) {
            int k = 0;
            for (int right = 0; right < 32; ++right) {
                if (valid_transition(left, mid, right))
                    trans_next[left][mid][k++] = (uint8_t)right;
            }
            trans_nnext[left][mid] = (uint8_t)k;
        }
    }
    trans_ready = true;
}

/* Main function: returns a(n) for n >= 1. */
seq_t a(idx_t n)
{
    build_trans();

    if (n == 0) return 0;   /* outside the defined range */

    /* Special case n == 1: only one column. */
    if (n == 1) {
        seq_t total = 0;
        for (int m = 0; m < 32; ++m)
            if (valid_transition(0, m, 0))
                total += (seq_t)1;
        return total;
    }

    /* DP tables: dp[prev][cur] – number of ways to reach a pair
       of consecutive columns with states prev and cur. */
    seq_t dp[32][32];
    seq_t next[32][32];
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp[i][j] = (seq_t)0;

    /* Initialise with the first two columns.  The leftmost neighbour
       is considered empty (0). */
    for (int m0 = 0; m0 < 32; ++m0) {
        for (int m1 = 0; m1 < 32; ++m1) {
            if (valid_transition(0, m0, m1))
                dp[m0][m1] = (seq_t)1;
        }
    }

    /* Process columns 2 .. n-1 (0‑based counting). */
    for (idx_t col = 2; col < n; ++col) {
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                next[i][j] = (seq_t)0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int cur = 0; cur < 32; ++cur) {
                seq_t ways = dp[prev][cur];
                if (!ways) continue;
                uint8_t *succ  = trans_next[prev][cur];
                uint8_t  cnt   = trans_nnext[prev][cur];
                for (int k = 0; k < cnt; ++k) {
                    int nxt = succ[k];
                    next[cur][nxt] += ways;
                }
            }
        }
        /* Move next to dp for the next iteration. */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = next[i][j];
    }

    /* Final column has no right neighbour (0).  Sum all valid endings. */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            seq_t ways = dp[prev][cur];
            if (!ways) continue;
            if (valid_transition(prev, cur, 0))
                result += ways;
        }
    }
    return result;
}
