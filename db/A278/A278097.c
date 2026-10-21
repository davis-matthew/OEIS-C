/*
 * A278097  Number of nX4 0..3 arrays with every element plus 1 mod 4 equal to
 *          some element at offset (-1,0) (-1,1) (0,-1) (0,1) or (1,0), with
 *          upper left element zero.
 * https://oeis.org/A278097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A278097:
   Count of n×4 arrays with entries 0..3, upper‑left fixed to 0,
   where for each cell its value+1 mod 4 equals at least one neighbor
   at offsets (-1,0),(-1,1),(0,-1),(0,1),(1,0).
   The algorithm uses dynamic programming over rows with a state
   (row pattern, unsatisfied‑mask) where the mask indicates which
   cells of the current row still lack a satisfied neighbor and
   therefore require the next row to provide it.  All transitions
   are pre‑computed for efficiency.  The code works for any
   integer type seq_t (8,16,32,64,128 bits or arbitrary precision)
   and any idx_t (unsigned 64‑bit).        */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* ---------- static data ---------- */
    static int init = 0;
    static uint8_t pat_val[256][4];
    static uint8_t unsat[256][256];
    static uint8_t groups[16][256][256];
    static uint16_t group_cnt[16][256];
    static seq_t dp[256 * 16];
    static seq_t dp_next[256 * 16];

    /* ---------- initialise static tables once ---------- */
    if (!init) {
        /* pattern values */
        for (int pat = 0; pat < 256; ++pat) {
            pat_val[pat][0] =  pat        & 3;
            pat_val[pat][1] = (pat >> 2)  & 3;
            pat_val[pat][2] = (pat >> 4)  & 3;
            pat_val[pat][3] = (pat >> 6)  & 3;
        }

        /* unsatisfied mask table: unsat[prev][cur] */
        for (int prev = 0; prev < 256; ++prev) {
            for (int cur = 0; cur < 256; ++cur) {
                uint8_t mask = 0;
                for (int j = 0; j < 4; ++j) {
                    uint8_t val   = pat_val[cur][j];
                    uint8_t need  = (val + 1) & 3;
                    unsigned sat  = 0;
                    /* left neighbour */
                    if (j > 0 && pat_val[cur][j-1] == need) sat = 1;
                    /* right neighbour */
                    if (!sat && j < 3 && pat_val[cur][j+1] == need) sat = 1;
                    /* up-left */
                    if (!sat && pat_val[prev][j] == need) sat = 1;
                    /* up-right */
                    if (!sat && j < 3 && pat_val[prev][j+1] == need) sat = 1;
                    if (!sat) mask |= (1U << j);
                }
                unsat[prev][cur] = mask;
            }
        }

        /* groups[mask][key] = list of patterns with the required values */
        for (int mask = 0; mask < 16; ++mask)
            for (int key = 0; key < 256; ++key)
                group_cnt[mask][key] = 0;

        for (int pat = 0; pat < 256; ++pat) {
            for (int mask = 0; mask < 16; ++mask) {
                int key = 0;
                for (int j = 0; j < 4; ++j)
                    if (mask & (1 << j))
                        key |= pat_val[pat][j] << (2 * j);
                uint16_t pos = group_cnt[mask][key];
                groups[mask][key][pos] = (uint8_t)pat;
                group_cnt[mask][key] = pos + 1;
            }
        }

        init = 1;
    }

    /* ---------- dp initialisation for the first row ---------- */
    for (int i = 0; i < 4096; ++i) dp[i] = 0;
    for (int pat = 0; pat < 256; ++pat) {
        if (pat_val[pat][0] != 0) continue;          /* upper‑left fixed to 0 */

        uint8_t mask = 0;
        for (int j = 0; j < 4; ++j) {
            uint8_t val  = pat_val[pat][j];
            uint8_t need = (val + 1) & 3;
            unsigned sat = 0;
            if (j > 0 && pat_val[pat][j-1] == need) sat = 1;
            if (!sat && j < 3 && pat_val[pat][j+1] == need) sat = 1;
            if (!sat) mask |= (1U << j);
        }
        dp[pat * 16 + mask] += 1;
    }

    /* ---------- process remaining rows ---------- */
    for (idx_t r = 1; r < n; ++r) {
        for (int i = 0; i < 4096; ++i) dp_next[i] = 0;

        for (int prev = 0; prev < 256; ++prev) {
            for (int pmask = 0; pmask < 16; ++pmask) {
                seq_t ways = dp[prev * 16 + pmask];
                if (ways == 0) continue;

                int key = 0;
                for (int j = 0; j < 4; ++j)
                    if (pmask & (1 << j)) {
                        int need = (pat_val[prev][j] + 1) & 3;
                        key |= need << (2 * j);
                    }

                uint16_t cnt = group_cnt[pmask][key];
                for (uint16_t k = 0; k < cnt; ++k) {
                    int cur = groups[pmask][key][k];
                    uint8_t cmask = unsat[prev][cur];
                    dp_next[cur * 16 + cmask] += ways;
                }
            }
        }

        /* swap dp and dp_next */
        for (int i = 0; i < 4096; ++i) dp[i] = dp_next[i];
    }

    /* ---------- sum up states with no unsatisfied cells ---------- */
    seq_t result = 0;
    for (int pat = 0; pat < 256; ++pat)
        result += dp[pat * 16 + 0];

    return result;
}
