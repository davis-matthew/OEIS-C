/*
 * A297547  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2 or 3 neighboring 1s.
 * https://oeis.org/A297547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   _valid_middle_column
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A297547:
 * Number of 4×n 0..1 arrays in which every 1 has 2 or 3 adjacent 1s
 * horizontally, diagonally or anti‑diagonally.  The definition is
 * implemented by a linear‑time dynamic‑programming algorithm that
 * stores the last two columns of the array.  No formula is known. */

#include <stdbool.h>
#include "../utils/_valid_middle_column.h"

/* Pre‑computed transition masks.  For every pair (left,middle)
 * _nextmask[left][middle] is a 16‑bit bitmask where bit `next` is 1
 * iff the triple (left, middle, next) satisfies the adjacency
 * requirement for all 1s in the middle column. */
static bool _init_done = false;
static unsigned short _nextmask[16][16];

static void _build_nextmask(void)
{
    if (_init_done) return;
    for (int left = 0; left < 16; ++left)
        for (int middle = 0; middle < 16; ++middle) {
            unsigned short mask = 0;
            for (int next = 0; next < 16; ++next)
                if (_valid_middle_column(left, middle, next))
                    mask |= 1u << next;
            _nextmask[left][middle] = mask;
        }
    _init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    _build_nextmask();

    /* dp[parity][prev][curr] – number of ways for the processed
     * columns ending with columns (prev,curr).  Only two layers
     * are required. */
    static seq_t dp[2][16][16];
    int cur = 0, nxt = 1;

    /* initialise: left of the first column is all zeros */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dp[cur][i][j] = 0;
    for (int first = 0; first < 16; ++first)
        dp[cur][0][first] = 1;

    /* process columns 2 … n */
    for (idx_t step = 1; step < n; ++step) {
        /* clear next layer */
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                dp[nxt][i][j] = 0;

        for (int left = 0; left < 16; ++left)
            for (int middle = 0; middle < 16; ++middle) {
                seq_t ways = dp[cur][left][middle];
                if (!ways) continue;
                unsigned short mask = _nextmask[left][middle];
                for (int next = 0; next < 16; ++next)
                    if (mask & (1u << next))
                        dp[nxt][middle][next] = dp[nxt][middle][next] + ways;
            }

        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* final count: last column must also satisfy the condition.
     * Use a dummy right column of all zeros. */
    seq_t ans = 0;
    for (int left = 0; left < 16; ++left)
        for (int curr = 0; curr < 16; ++curr) {
            seq_t ways = dp[cur][left][curr];
            if (!ways) continue;
            if (_valid_middle_column(left, curr, 0))
                ans = ans + ways;
        }
    return ans;
}
