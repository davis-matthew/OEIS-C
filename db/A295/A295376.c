/*
 * A295376  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 2 or 4 1s.
 * https://oeis.org/A295376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP computation of A295376:  Number of n×5 0..1 arrays with each 1 horizontally
 * or vertically adjacent to 0, 2 or 4 1s.  The DP keeps the last two row
 * patterns (previous and current).  A transition from (q,p) to (p,r) is
 * allowed iff every 1 in row p has an even number of 1‑neighbours
 * (up from q, down from r, and left/right in p).  Precomputation of the
 * 32×32 transition masks turns the O(32^3) per row cost into
 * ~10k additions per row (≈ 10⁷ for n=10 000). */
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Precompute validation masks once. */
    static bool initialized = false;
    static uint32_t validMask[32][32];  /* bit r is set if (q,p,r) is valid */

    if (!initialized) {
        for (int q = 0; q < 32; ++q) {
            for (int p = 0; p < 32; ++p) {
                uint32_t mask = 0;
                for (int r = 0; r < 32; ++r) {
                    bool ok = true;
                    for (int j = 0; j < 5; ++j) {
                        if ((p >> j) & 1) {          /* cell is 1 */
                            int up    = (q >> j) & 1;
                            int down  = (r >> j) & 1;
                            int left  = (j > 0)    ? ((p >> (j - 1)) & 1) : 0;
                            int right = (j < 4)    ? ((p >> (j + 1)) & 1) : 0;
                            if ((up ^ down ^ left ^ right) != 0) {
                                ok = false;
                                break;
                            }
                        }
                    }
                    if (ok)
                        mask |= 1u << r;
                }
                validMask[q][p] = mask;
            }
        }
        initialized = true;
    }

    if (n == 0) {
        return 0;
    }

    /* DP arrays: cur[prev][cur], next[prev][cur] */
    static seq_t cur[32][32];
    static seq_t next[32][32];

    /* Zero initialise cur */
    for (int i = 0; i < 32; ++i)
        for (int j = 0; j < 32; ++j)
            cur[i][j] = 0;

    /* Special case n == 1: only one row, no up/down neighbours */
    if (n == 1) {
        seq_t ans = 0;
        for (int p = 0; p < 32; ++p) {
            if ((validMask[0][p] & 1u) != 0) {   /* r == 0 */
                ans += 1;
            }
        }
        return ans;
    }

    /* Initialization for the first two rows */
    for (int p1 = 0; p1 < 32; ++p1) {
        uint32_t mask = validMask[0][p1];          /* q = 0 (top border) */
        for (int p2 = 0; p2 < 32; ++p2) {
            if ((mask >> p2) & 1u) {
                cur[p1][p2] += 1;
            }
        }
    }

    /* Process rows 3 .. n */
    for (idx_t i = 3; i <= n; ++i) {
        /* Zero initialise next */
        for (int i1 = 0; i1 < 32; ++i1)
            for (int j1 = 0; j1 < 32; ++j1)
                next[i1][j1] = 0;

        for (int q = 0; q < 32; ++q) {
            for (int p = 0; p < 32; ++p) {
                seq_t count = cur[q][p];
                if (count == 0)
                    continue;
                uint32_t mask = validMask[q][p];
                uint32_t m = mask;
                while (m) {
                    int r = __builtin_ctz(m);
                    m &= m - 1;
                    next[p][r] += count;
                }
            }
        }
        /* swap cur and next */
        for (int i1 = 0; i1 < 32; ++i1)
            for (int j1 = 0; j1 < 32; ++j1)
                cur[i1][j1] = next[i1][j1];
    }

    /* Final bottom row constraint: row n must satisfy even neighbours
     * with up neighbour from row n-1 and no down neighbour. */
    seq_t ans = 0;
    for (int q = 0; q < 32; ++q) {
        for (int p = 0; p < 32; ++p) {
            if ((validMask[q][p] & 1u) != 0) {  /* r == 0 */
                ans += cur[q][p];
            }
        }
    }
    return ans;
}
