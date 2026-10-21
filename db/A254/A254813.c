/*
 * A254813  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock sum of
 *          four medians of central row, central column, diagonal and
 *          antidiagonal nondecreasing horizontally and vertically.
 * https://oeis.org/A254813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+2)×4 binary arrays where every 3×3 subblock
 *            satisfies that the sum of the four medians (central row, central
 *            column, main diagonal, anti-diagonal) is nondecreasing
 *            horizontally (sum[j] ≤ sum[j+1]) and vertically (sum[i] ≤
 *            sum[i+1]) in the grid of subblocks. A dynamic programmi...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned char sum0[4096];
static unsigned char sum1[4096];

/* median of three bits */
static inline unsigned char median3(unsigned char a, unsigned char b, unsigned char c)
{
    return ((a + b + c) >= 2) ? 1 : 0;
}

/* pre–compute the four subblock sums for every triple of rows */
static void precompute_sums(void)
{
    for (unsigned int a = 0; a < 16; ++a)
    {
        for (unsigned int b = 0; b < 16; ++b)
        {
            for (unsigned int c = 0; c < 16; ++c)
            {
                unsigned int idx = (a << 8) | (b << 4) | c;

                /* j = 0 */
                unsigned char m0_row = median3((b >> 0) & 1, (b >> 1) & 1, (b >> 2) & 1);
                unsigned char m0_col = median3((a >> 1) & 1, (b >> 1) & 1, (c >> 1) & 1);
                unsigned char m0_diag = median3((a >> 0) & 1, (b >> 1) & 1, (c >> 2) & 1);
                unsigned char m0_antidiag = median3((a >> 2) & 1, (b >> 1) & 1, (c >> 0) & 1);
                unsigned char s0 = m0_row + m0_col + m0_diag + m0_antidiag;

                /* j = 1 */
                unsigned char m1_row = median3((b >> 1) & 1, (b >> 2) & 1, (b >> 3) & 1);
                unsigned char m1_col = median3((a >> 2) & 1, (b >> 2) & 1, (c >> 2) & 1);
                unsigned char m1_diag = median3((a >> 1) & 1, (b >> 2) & 1, (c >> 3) & 1);
                unsigned char m1_antidiag = median3((a >> 3) & 1, (b >> 2) & 1, (c >> 1) & 1);
                unsigned char s1 = m1_row + m1_col + m1_diag + m1_antidiag;

                sum0[idx] = s0;
                sum1[idx] = s1;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* the array size is (n+2) rows by 4 columns */
    idx_t N = n + 2;                 /* number of rows */
    /* DP arrays: 3‑row states, 16×16×16 = 4096 states */
    static seq_t dp_cur[4096];
    static seq_t dp_nxt[4096];
    static bool initialized = false;
    if (!initialized)
    {
        precompute_sums();
        initialized = true;
    }

    /* base cases: first three rows */
    for (int i = 0; i < 4096; ++i)
    {
        seq_t val = 0;
        if (sum0[i] <= sum1[i])
            val = 1;
        dp_cur[i] = val;
    }

    /* process remaining rows (index 3 .. N-1) */
    for (idx_t step = 3; step < N; ++step)
    {
        /* zero next array */
        for (int i = 0; i < 4096; ++i)
            dp_nxt[i] = 0;

        for (int idx = 0; idx < 4096; ++idx)
        {
            seq_t ways = dp_cur[idx];
            if (ways == 0)
                continue;

            unsigned char prev_s0 = sum0[idx];
            unsigned char prev_s1 = sum1[idx];

            unsigned int b = (idx >> 4) & 0xF;
            unsigned int c = idx & 0xF;

            /* try all possible next rows */
            for (unsigned int d = 0; d < 16; ++d)
            {
                unsigned int new_idx = (b << 8) | (c << 4) | d;
                unsigned char new_s0 = sum0[new_idx];
                unsigned char new_s1 = sum1[new_idx];

                if (new_s0 >= prev_s0 && new_s1 >= prev_s1 && new_s0 <= new_s1)
                {
                    dp_nxt[new_idx] += ways;
                }
            }
        }

        /* swap arrays */
        for (int i = 0; i < 4096; ++i)
            dp_cur[i] = dp_nxt[i];
    }

    /* final answer: sum of all DP states for the full array */
    seq_t ans = 0;
    for (int i = 0; i < 4096; ++i)
        ans += dp_cur[i];

    return ans;
}
