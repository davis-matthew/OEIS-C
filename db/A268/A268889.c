/*
 * A268889  Number of 4Xn binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A268889
 *  Number of 4×n binary arrays where exactly one horizontally or
 *  anti-diagonally adjacent pair of ones occurs.
 *
 *  The computation uses a column‑by‑column dynamic program:
 *      dp0[p] = number of ways to reach pattern p with 0 such pairs
 *      dp1[p] = number of ways to reach pattern p with 1 such pair
 *  Transition (p → q) adds f(p,q) pairs, where f(p,q) is the number
 *  of 1‑1 pairs shared by the two columns (horizontal same‑row plus
 *  anti‑diagonal upper‑right).  Only transitions with f(p,q) ≤ 1
 *  contribute to the next state.
 *
 *  Complexity: O(16²·n) ≈ 256n operations, easily under 2 s for n≤10 000.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset = 1 */

    /* Pre‑compute pair counts between column patterns. */
    static unsigned char f[16][16];
    static int f_done = 0;
    if (!f_done) {
        for (int p = 0; p < 16; ++p) {
            for (int q = 0; q < 16; ++q) {
                unsigned int horiz = __builtin_popcount((uint16_t)p & (uint16_t)q);
                unsigned int diag  = __builtin_popcount(((uint16_t)p << 1) & (uint16_t)q);
                f[p][q] = (unsigned char)(horiz + diag);   /* 0 .. 8 */
            }
        }
        f_done = 1;
    }

    seq_t dp0[16], dp1[16], ndp0[16], ndp1[16];

    /* Initial column (no previous neighbor). */
    for (int i = 0; i < 16; ++i) {
        dp0[i] = 1;   /* one way to have pattern i with 0 pairs */
        dp1[i] = 0;   /* impossible to have a pair yet */
    }

    /* Process remaining n−1 transitions. */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < 16; ++i) ndp0[i] = ndp1[i] = 0;

        for (int p = 0; p < 16; ++p) {
            seq_t ways0 = dp0[p];
            seq_t ways1 = dp1[p];
            if (!ways0 && !ways1) continue;

            for (int q = 0; q < 16; ++q) {
                unsigned char cnt = f[p][q];
                if (cnt == 0) {
                    ndp0[q] += ways0;
                    ndp1[q] += ways1;
                } else if (cnt == 1) {
                    ndp1[q] += ways0;
                    /* ways1 cannot go to ndp0; would exceed 1 */
                }
                /* cnt > 1: ignore transition (would produce >1 pairs) */
            }
        }

        for (int i = 0; i < 16; ++i) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
        }
    }

    /* Sum over all ending patterns with exactly one pair. */
    seq_t ans = 0;
    for (int i = 0; i < 16; ++i) ans += dp1[i];
    return ans;
}
