/*
 * A283946  Number of nX4 0..1 arrays with no 1 equal to more than three of
 *          its horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over rows. Number of n×4 binary arrays with exactly one ‘1’
 *            that has more than three of its horizontal, diagonal and
 *            antidiagonal neighbors also equal to 1. All other 1’s must have
 *            at most three equal neighbors. The DP counts arrays for each n.
 *            The recurrence is derived from the definition, no...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 198 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned char compute_violations(unsigned prev2, unsigned prev1, unsigned cur)
{
    unsigned violations = 0;
    for (int c = 0; c < 4; ++c) {
        unsigned cell = (prev1 >> c) & 1U;
        if (cell == 0)
            continue;               /* The rule applies only to 1’s. */
        unsigned equal = 0;
        /* horizontal neighbors */
        if (c > 0)   equal += ((prev1 >> (c - 1)) & 1U) == cell;
        if (c < 3)   equal += ((prev1 >> (c + 1)) & 1U) == cell;
        /* diagonal neighbors (up-left, up-right) */
        if (c > 0)   equal += ((prev2 >> (c - 1)) & 1U) == cell;
        if (c < 3)   equal += ((prev2 >> (c + 1)) & 1U) == cell;
        /* antidiagonal neighbors (down-left, down-right) */
        if (c > 0)   equal += ((cur   >> (c - 1)) & 1U) == cell;
        if (c < 3)   equal += ((cur   >> (c + 1)) & 1U) == cell;
        if (equal > 3) violations++;
    }
    return (unsigned char)violations;
}

static unsigned char viol[256][16];
static bool precomputed = false;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* Not in the official domain. */

    /* Precompute violation table once. */
    if (!precomputed) {
        for (unsigned p2 = 0; p2 < 16; ++p2) {
            for (unsigned p1 = 0; p1 < 16; ++p1) {
                unsigned state = (p2 << 4) | p1;
                for (unsigned cur = 0; cur < 16; ++cur) {
                    viol[state][cur] = compute_violations(p2, p1, cur);
                }
            }
        }
        precomputed = true;
    }

    /* DP arrays: dp_cur[state][used] where
       state encodes two consecutive rows (prev2<<4 | prev1),
       used = 0 or 1 (number of violations found). */
    static seq_t dp_cur[256][2];
    static seq_t dp_next[256][2];
    mem_set(dp_cur, 0, sizeof dp_cur);

    /* First row: prev2 = 0, prev1 = pattern of the first row */
    for (int p1 = 0; p1 < 16; ++p1) {
        int state = p1;                    /* prev2==0, prev1==p1 */
        dp_cur[state][0] = dp_cur[state][0] + (seq_t)1;
    }

    /* Process additional rows (row 2 to n). */
    for (idx_t r = 2; r <= n; ++r) {
        mem_set(dp_next, 0, sizeof dp_next);
        for (int state = 0; state < 256; ++state) {
            unsigned prev2 = (state >> 4) & 0xF;
            unsigned prev1 = state & 0xF;
            for (int used = 0; used < 2; ++used) {
                seq_t val = dp_cur[state][used];
                if (val == 0)
                    continue;
                for (int cur = 0; cur < 16; ++cur) {
                    unsigned add = viol[(prev2 << 4) | prev1][cur];
                    int new_used = used + add;
                    if (new_used > 1)
                        continue;
                    int new_state = (prev1 << 4) | cur;
                    dp_next[(unsigned)new_state][new_used] += val;
                }
            }
        }
        /* swap dp_cur and dp_next */
        mem_copy(dp_cur, dp_next, sizeof dp_cur);
    }

    /* Final evaluation: use a dummy zero row to finish the last real row. */
    seq_t total = (seq_t)0;
    unsigned dummy = 0;
    for (int state = 0; state < 256; ++state) {
        unsigned prev2 = (state >> 4) & 0xF;
        unsigned prev1 = state & 0xF;
        for (int used = 0; used < 2; ++used) {
            seq_t val = dp_cur[state][used];
            if (val == 0)
                continue;
            unsigned add = viol[(prev2 << 4) | prev1][dummy];
            int final_used = used + add;
            if (final_used == 1)
                total += val;
        }
    }
    return total;
}
