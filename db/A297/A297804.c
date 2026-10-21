/*
 * A297804  Number of 4Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 2, 3 or 5 neighboring 1s.
 * https://oeis.org/A297804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays such that every 1 has exactly
 *            2, 3, or 5 neighboring 1s in the six directions (horizontal and
 *            diagonal). Computed by dynamic programming
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper table: _a297804_valid[p][c][n] is true iff for the triple
   (p,c,n) of consecutive columns every 1 in column c has a neighbour
   count of 2, 3, or 5 using only columns p (left) and n (right). */
static unsigned char _a297804_valid[16][16][16];
static bool _a297804_ready = false;

static void _a297804_precompute(void)
{
    for (int p = 0; p < 16; ++p) {
        for (int c = 0; c < 16; ++c) {
            for (int n = 0; n < 16; ++n) {
                bool ok = true;
                for (int r = 0; r < 4 && ok; ++r) {
                    if ((c >> r) & 1) {
                        int cnt = 0;
                        /* left neighbours from column p */
                        if ((p >> r) & 1) ++cnt;
                        if (r > 0 && (p >> (r - 1)) & 1) ++cnt;
                        if (r < 3 && (p >> (r + 1)) & 1) ++cnt;
                        /* right neighbours from column n */
                        if ((n >> r) & 1) ++cnt;
                        if (r > 0 && (n >> (r - 1)) & 1) ++cnt;
                        if (r < 3 && (n >> (r + 1)) & 1) ++cnt;
                        if (!(cnt == 2 || cnt == 3 || cnt == 5))
                            ok = false;
                    }
                }
                _a297804_valid[p][c][n] = ok;
            }
        }
    }
    _a297804_ready = true;
}

seq_t a(idx_t n)
{
    if (!_a297804_ready)
        _a297804_precompute();

    /* dp[state] where state = (p << 4) | c */
    seq_t dp[256] = {0};
    dp[0] = 1;                     /* before first column: left dummy 0 */

    for (idx_t col = 1; col <= n; ++col) {
        seq_t newdp[256] = {0};
        for (unsigned s = 0; s < 256; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            unsigned p = s >> 4;    /* column p (previous) */
            unsigned c = s & 0xF;   /* column c (current) */
            for (unsigned nt = 0; nt < 16; ++nt) {
                if (_a297804_valid[p][c][nt]) {
                    unsigned ns = (c << 4) | nt;
                    newdp[ns] += cur;
                }
            }
        }
        for (unsigned i = 0; i < 256; ++i) dp[i] = newdp[i];
    }

    /* final dummy column 0 on the right */
    seq_t ans = 0;
    for (unsigned s = 0; s < 256; ++s) {
        seq_t cur = dp[s];
        if (!cur) continue;
        unsigned p = s >> 4;
        unsigned c = s & 0xF;
        if (_a297804_valid[p][c][0])
            ans += cur;
    }
    return ans;
}
