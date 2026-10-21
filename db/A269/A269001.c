/*
 * A269001  Number of 7Xn binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A269001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays with at most one horizontal
 *            or anti‑diagonal adjacent pair of 1s (sum 2) in the entire array
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t dp0[128];
    static seq_t dp1[128];
    static seq_t ndp0[128];
    static seq_t ndp1[128];
    static unsigned char added[128][128];
    static int init = 0;

    /* Precompute number of new adjacent‑one pairs for each pattern pair. */
    if (!init) {
        for (idx_t prev = 0; prev < 128; ++prev) {
            unsigned int p = (unsigned int)prev;
            for (idx_t cur = 0; cur < 128; ++cur) {
                unsigned int c = (unsigned int)cur;
                unsigned int h  = __builtin_popcount(p & c);               /* horizontal */
                unsigned int ad = __builtin_popcount(((p << 1) & 0x7E) & c); /* anti‑diagonal */
                unsigned int val = h + ad;
                added[prev][cur] = (val > 1) ? 2 : (unsigned char)val;      /* 0,1,or>1 */
            }
        }
        init = 1;
    }

    /* Initialise DP for the first column: any pattern, no pair used yet. */
    for (idx_t i = 0; i < 128; ++i) {
        dp0[i] = 0;
        dp1[i] = 0;
    }
    for (idx_t pat = 0; pat < 128; ++pat) {
        dp0[pat] = 1;
    }

    /* Process remaining columns. */
    for (idx_t col = 1; col < n; ++col) {
        /* Zero next‑state arrays. */
        for (idx_t i = 0; i < 128; ++i) {
            ndp0[i] = 0;
            ndp1[i] = 0;
        }

        for (idx_t prev = 0; prev < 128; ++prev) {
            seq_t v0 = dp0[prev];
            seq_t v1 = dp1[prev];
            if (v0 == 0 && v1 == 0) continue;

            for (idx_t cur = 0; cur < 128; ++cur) {
                unsigned char add = added[prev][cur];
                if (add == 2) continue; /* would create more than one pair */

                if (v0 != 0) {
                    if (add == 0) ndp0[cur] += v0;
                    else          ndp1[cur] += v0; /* add == 1 */
                }
                if (v1 != 0 && add == 0) ndp1[cur] += v1;
            }
        }

        /* Copy new states back to dp arrays. */
        for (idx_t i = 0; i < 128; ++i) {
            dp0[i] = ndp0[i];
            dp1[i] = ndp1[i];
        }
    }

    /* Sum all valid end states. */
    seq_t result = 0;
    for (idx_t i = 0; i < 128; ++i) {
        result += dp0[i];
        result += dp1[i];
    }
    return result;
}
