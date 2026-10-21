/*
 * A283279  Number of nX5 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A283279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts n×5 binary arrays in which each ‘1’ has no more than
 *            two neighbours among horizontal, vertical, and antidiagonal
 *            (up‑right, down‑left) directions. The count is obtained by a
 *            dynamic program over 5‑bit row states
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


static uint32_t allowed_mask[32][32];
static bool      last_valid[32][32];
static bool      tables_ok = false;

/* Returns true iff, given the rows above (a) and below (c),
   the middle row (b) satisfies the neighbour restriction. */
static inline bool triple_ok(int a, int b, int c)
{
    for (int j = 0; j < 5; ++j) {
        if ((b >> j) & 1U) {
            int cnt = 0;
            /* horizontal neighbours */
            if (j > 0     && ((b >> (j - 1)) & 1U)) ++cnt;
            if (j < 4     && ((b >> (j + 1)) & 1U)) ++cnt;
            /* vertical neighbours */
            if ((a >>  j) & 1U) ++cnt;          /* above */
            if ((c >>  j) & 1U) ++cnt;          /* below */
            /* antidiagonal neighbours */
            if (j < 4     && ((a >> (j + 1)) & 1U)) ++cnt;
            if (j > 0     && ((c >> (j - 1)) & 1U)) ++cnt;
            if (cnt > 2)   return false;
        }
    }
    return true;
}

/* Build the 32×32 transition tables. */
static void precompute(void)
{
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            uint32_t mask = 0;
            for (int c = 0; c < 32; ++c) {
                if (triple_ok(a, b, c))
                    mask |= 1U << c;
            }
            allowed_mask[a][b] = mask;
            last_valid[a][b]   = triple_ok(a, b, 0);
        }
    }
    tables_ok = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!tables_ok) precompute();

    if (n == 0) return 0;          /* offset is 1, behaviour for 0 is irrelevant */

    /* DP tables for two consecutive rows. */
    seq_t dp_cur[32][32];
    seq_t dp_next[32][32];

    /* initialise to zero */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            dp_cur[i][j] = 0;

    /* first row: above row is all zeros. */
    for (int r = 0; r < 32; ++r)
        dp_cur[0][r] = 1;

    /* iterate over rows 2 … n, validating row i‑1 each time. */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_next[i][j] = 0;

        for (int pm = 0; pm < 32; ++pm) {            /* row i‑2 */
            for (int pr = 0; pr < 32; ++pr) {        /* row i‑1 */
                seq_t val = dp_cur[pm][pr];
                if (!val) continue;
                uint32_t mask = allowed_mask[pm][pr];
                for (int nr = 0; nr < 32; ++nr) {    /* row i */
                    if (mask & (1U << nr))
                        dp_next[pr][nr] += val;
                }
            }
        }
        /* move dp_next → dp_cur for next iteration */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* final row check: below row is all zeros. */
    seq_t total = 0;
    for (int pm = 0; pm < 32; ++pm) {
        for (int pr = 0; pr < 32; ++pr) {
            if (last_valid[pm][pr])
                total += dp_cur[pm][pr];
        }
    }
    return total;
}
