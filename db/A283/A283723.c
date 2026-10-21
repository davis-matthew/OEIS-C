/*
 * A283723  Number of n X 5 0..1 arrays with no 1 equal to more than two of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×5 binary arrays with exactly one 1 allowed to have
 *            more than two horizontal + diagonal + antidiagonal 1‑neighbors.
 *            Computed by a transfer‑matrix DP over the rows; each row is a
 *            5‑bit pattern. The DP keeps a flag meaning whether the unique
 *            exception has already been used
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the algorithm works for all n≥1; it uses at most 2·32·32 seq_t values
   per iteration, which is <2000. For n≤ 10000 it runs comfortably below
   the time limit. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char violation[32][32][32] = {0};

static void init_violation_table(void)
{
    for (int prev = 0; prev < 32; ++prev) {
        for (int cur = 0; cur < 32; ++cur) {
            for (int next = 0; next < 32; ++next) {
                unsigned v = 0;
                for (int j = 0; j < 5; ++j) {
                    if ((cur >> j) & 1) {
                        unsigned sum = 0;
                        /* horizontal */
                        if (j > 0)   sum += (cur >> (j - 1)) & 1;
                        if (j < 4)   sum += (cur >> (j + 1)) & 1;
                        /* diagonal from top row */
                        if (j > 0)   sum += (prev >> (j - 1)) & 1;
                        if (j < 4)   sum += (prev >> (j + 1)) & 1;
                        /* antidiagonal from bottom row */
                        if (j > 0)   sum += (next >> (j - 1)) & 1;
                        if (j < 4)   sum += (next >> (j + 1)) & 1;
                        if (sum > 2) {
                            ++v;
                            if (v > 1) { v = 2; break; }
                        }
                    }
                }
                violation[prev][cur][next] = (unsigned char)v; /* 0,1,or 2 */
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* initialize violation table once */
    static bool init = false;
    if (!init) {
        init = true;
        init_violation_table();
    }

    /* DP arrays */
    seq_t dp[2][32][32] = {0};
    seq_t newdp[2][32][32];

    /* initial state: first row pattern, previous row is 0 (border) */
    for (int cur = 0; cur < 32; ++cur) dp[0][0][cur] = 1;

    /* iterate over rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero newdp */
        for (int f = 0; f < 2; ++f)
            for (int p = 0; p < 32; ++p)
                for (int c = 0; c < 32; ++c)
                    newdp[f][p][c] = 0;

        for (int f = 0; f < 2; ++f) {
            for (int p = 0; p < 32; ++p) {
                for (int c = 0; c < 32; ++c) {
                    seq_t val = dp[f][p][c];
                    if (!val) continue;
                    for (int nrow = 0; nrow < 32; ++nrow) {
                        unsigned char v = violation[p][c][nrow];
                        if (v == 2) continue;           /* too many violations */
                        if (v == 0) {
                            newdp[f][c][nrow] += val;     /* no new violation */
                        } else { /* v == 1 */
                            if (f == 0) newdp[1][c][nrow] += val;
                        }
                    }
                }
            }
        }

        /* copy newdp into dp for next iteration */
        for (int f = 0; f < 2; ++f)
            for (int p = 0; p < 32; ++p)
                for (int c = 0; c < 32; ++c)
                    dp[f][p][c] = newdp[f][p][c];
    }

    /* final step: evaluate the last row (cur) with bottom border 0 */
    seq_t result = 0;
    int bottom = 0;
    for (int f = 0; f < 2; ++f) {
        for (int p = 0; p < 32; ++p) {
            for (int c = 0; c < 32; ++c) {
                seq_t val = dp[f][p][c];
                if (!val) continue;
                unsigned char v = violation[p][c][bottom];
                if (v == 2) continue;                   /* invalid */
                int final_flag = f;
                if (v == 1) {
                    if (f == 0) final_flag = 1;          /* use the exception */
                    else continue;                       /* second violation */
                }
                if (final_flag == 1) result += val;
            }
        }
    }

    return result;
}
