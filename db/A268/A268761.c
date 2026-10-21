/*
 * A268761  Number of n X 3 binary arrays with some element plus some
 *          horizontally, vertically, diagonally or antidiagonally adjacent
 *          neighbor totalling two exactly once.
 * https://oeis.org/A268761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n × 3 binary arrays containing exactly one
 *            adjacent pair of 1s (adjacency includes horizontal, vertical,
 *            diagonal, and anti‑diagonal neighbours). The function counts all
 *            such arrays by dynamic programming over the rows
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* seq_t and idx_t are defined by the surrounding build. */

seq_t a(idx_t n)
{
    /* return 0 for undefined index n==0 (sequence offset is 1). */
    if (n == 0)
        return (seq_t)0;

    /* Pre‑computed number of horizontal edges inside a row for each pattern. */
    static const int int_edges[8] = {0, 0, 0, 1, 0, 0, 1, 2};

    /* Pre‑compute cross‑row adjacency counts for all pattern pairs. */
    static int adjacency[8][8];
    static int adjacency_initialized = 0;
    if (!adjacency_initialized) {
        for (int p = 0; p < 8; ++p) {
            for (int q = 0; q < 8; ++q) {
                int acount = 0;
                for (int c = 0; c < 3; ++c)
                    if (p & (1 << c))
                        for (int d = c - 1; d <= c + 1; ++d)
                            if (d >= 0 && d < 3 && (q & (1 << d)))
                                ++acount;
                adjacency[p][q] = acount;
            }
        }
        adjacency_initialized = 1;
    }

    /* DP arrays: dp[pattern][flag] where flag=0 (no edge yet) or 1 (exactly one). */
    seq_t dp[8][2] = {{0}};
    seq_t newdp[8][2];

    /* Initialise for the first row. */
    for (int p = 0; p < 8; ++p) {
        int e = int_edges[p];
        if (e <= 1) {
            dp[p][e] = (seq_t)1;
        }
    }

    /* Process subsequent rows. */
    for (idx_t row = 1; row < n; ++row) {
        /* Reset newdp. */
        for (int q = 0; q < 8; ++q) {
            newdp[q][0] = (seq_t)0;
            newdp[q][1] = (seq_t)0;
        }

        /* Transition from previous row patterns. */
        for (int p = 0; p < 8; ++p) {
            for (int f = 0; f <= 1; ++f) {
                seq_t val = dp[p][f];
                if (val == (seq_t)0)
                    continue;

                for (int q = 0; q < 8; ++q) {
                    int e = int_edges[q];
                    int a = adjacency[p][q];
                    int total = f + e + a;
                    if (total <= 1) {
                        newdp[q][total] += val;
                    }
                }
            }
        }

        /* Copy newdp to dp for next iteration. */
        for (int q = 0; q < 8; ++q) {
            dp[q][0] = newdp[q][0];
            dp[q][1] = newdp[q][1];
        }
    }

    /* Sum all patterns with exactly one edge. */
    seq_t ans = (seq_t)0;
    for (int p = 0; p < 8; ++p)
        ans += dp[p][1];

    return ans;
}
