/*
 * A251353  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than the absolute difference of its
 *          antidiagonal elements.
 * https://oeis.org/A251353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 such that for
 *            every 2×2 subblock the maximum of its diagonal elements is not
 *            less than the absolute difference of its antidiagonal elements
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP enumerates rows (each row has 3 digits 0..3 = 4^3 = 64 states).
   For each pair of consecutive rows we precompute whether the 2×2
   subblocks formed between them satisfy the condition, and then run a
   linear‑time DP over the n+1 rows. */
/* Complexity: O(n·64²) ≈ 4 096 n operations, well within 2 s for n ≈ 10 000. */
#include <stdint.h>
#include <stdbool.h>

#define ROWS 64  /* 4^3 possible rows */

static uint8_t next_rows[ROWS][ROWS];
static uint8_t next_cnt[ROWS];
static uint8_t row_digits[ROWS][3];
static bool adj_built = false;

static seq_t dp_cur[ROWS];
static seq_t dp_next[ROWS];

static void build_adjacency(void)
{
    /* decompose each row into its 3 digits (columns 0..2) */
    for (int u = 0; u < ROWS; ++u) {
        for (int j = 0; j < 3; ++j)
            row_digits[u][j] = (u >> (2 * j)) & 3;
    }

    /* precompute transitions */
    for (int u = 0; u < ROWS; ++u) {
        next_cnt[u] = 0;
        for (int v = 0; v < ROWS; ++v) {
            bool ok = true;
            for (int j = 0; j < 2; ++j) {  /* two 2×2 blocks per pair of rows */
                uint8_t a = row_digits[u][j];
                uint8_t b = row_digits[u][j + 1];
                uint8_t c = row_digits[v][j];
                uint8_t d = row_digits[v][j + 1];
                uint8_t maxDiag = (a > d) ? a : d;
                uint8_t diff = (b > c) ? (b - c) : (c - b);
                if (maxDiag < diff) { ok = false; break; }
            }
            if (ok) {
                next_rows[u][next_cnt[u]++] = (uint8_t)v;
            }
        }
    }
    adj_built = true;
}

seq_t a(idx_t n)
{
    if (!adj_built)
        build_adjacency();

    /* initialize: any first row is allowed */
    for (int u = 0; u < ROWS; ++u)
        dp_cur[u] = (seq_t)1;

    for (idx_t step = 1; step <= n; ++step) {
        /* reset dp_next */
        for (int v = 0; v < ROWS; ++v)
            dp_next[v] = (seq_t)0;

        /* transition from current row to next */
        for (int u = 0; u < ROWS; ++u) {
            seq_t val = dp_cur[u];
            if (val == (seq_t)0) continue;
            for (int k = 0; k < next_cnt[u]; ++k) {
                int v = next_rows[u][k];
                dp_next[v] += val;
            }
        }

        if (step == n) {
            seq_t total = (seq_t)0;
            for (int v = 0; v < ROWS; ++v)
                total += dp_next[v];
            return total;
        }

        /* prepare for next iteration */
        for (int v = 0; v < ROWS; ++v)
            dp_cur[v] = dp_next[v];
    }

    return (seq_t)0;  /* n==0 is outside the offset, return 0 */
}
