/*
 * A303886  Number of nX6 0..1 arrays with every element unequal to 0, 2, 3 or
 *          4 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A303886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A303886 by dynamic programming over 6×n binary arrays
   with the king‑move neighbour constraint.
   The constraint is: for each element, the number of king‑move adjacent
   elements that differ from it must be in the set {0,2,3,4}.
   The array has upper‑left element 0. */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Utility: test if the element on a row satisfies the neighbour rule.
   row : the current 6‑bit row.
   prev: the row above (0 if none).
   next: the row below (0 if none).
   has_prev, has_next: whether prev/next exist. */
static inline bool
check_row(uint8_t row, uint8_t prev, uint8_t next,
          bool has_prev, bool has_next)
{
    for (int c = 0; c < 6; ++c) {
        uint8_t bit_c = (row >> c) & 1;
        int unequal = 0;

        /* left neighbour */
        if (c > 0) {
            uint8_t bit = (row >> (c-1)) & 1;
            if (bit != bit_c) ++unequal;
        }
        /* right neighbour */
        if (c < 5) {
            uint8_t bit = (row >> (c+1)) & 1;
            if (bit != bit_c) ++unequal;
        }

        /* up‑left, up, up‑right */
        if (has_prev) {
            if (c > 0) {
                uint8_t bit = (prev >> (c-1)) & 1;
                if (bit != bit_c) ++unequal;
            }
            {
                uint8_t bit = (prev >> c) & 1;
                if (bit != bit_c) ++unequal;
            }
            if (c < 5) {
                uint8_t bit = (prev >> (c+1)) & 1;
                if (bit != bit_c) ++unequal;
            }
        }

        /* down‑left, down, down‑right */
        if (has_next) {
            if (c > 0) {
                uint8_t bit = (next >> (c-1)) & 1;
                if (bit != bit_c) ++unequal;
            }
            {
                uint8_t bit = (next >> c) & 1;
                if (bit != bit_c) ++unequal;
            }
            if (c < 5) {
                uint8_t bit = (next >> (c+1)) & 1;
                if (bit != bit_c) ++unequal;
            }
        }

        /* allow only counts 0,2,3,4 */
        if (!(unequal == 0 || unequal == 2 || unequal == 3 || unequal == 4))
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset is 1 – term 0 unused */

    /* one‑time pre‑computation of transition data */
    static bool init = false;
    static uint64_t start_mask[64];
    static uint64_t trans_mask[64][64];
    static bool last_valid[64][64];

    if (!init) {
        for (int p = 0; p < 64; ++p) {
            uint64_t sm = 0;
            for (int q = 0; q < 64; ++q)
                if (check_row((uint8_t)p, 0, (uint8_t)q, false, true))
                    sm |= 1ULL << q;
            start_mask[p] = sm;
        }

        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                uint64_t mask = 0;
                for (int nxt = 0; nxt < 64; ++nxt)
                    if (check_row((uint8_t)curr,
                                  (uint8_t)prev,
                                  (uint8_t)nxt,
                                  true, true))
                        mask |= 1ULL << nxt;
                trans_mask[prev][curr] = mask;
                last_valid[prev][curr] =
                    check_row((uint8_t)curr,
                              (uint8_t)prev, 0,
                              true, false);
            }
        }
        init = true;
    }

    /* n == 1 : only the first row exists; it must satisfy its own constraint. */
    if (n == 1) {
        seq_t total = 0;
        for (int row = 0; row < 64; ++row) {
            if (row & 1) continue;                 /* upper‑left must be 0 */
            if (check_row((uint8_t)row, 0, 0, false, false))
                ++total;
        }
        return total;
    }

    /* dynamic programming over the pair of last two rows */
    static seq_t dp[64][64];
    static seq_t ndp[64][64];
    memset(dp, 0, sizeof(dp));

    /* initialise with the first two rows */
    for (int row0 = 0; row0 < 64; ++row0) {
        if (row0 & 1) continue;                 /* upper‑left 0 */
        uint64_t mask = start_mask[row0];
        while (mask) {
            int row1 = __builtin_ctzll(mask);
            mask &= mask - 1;
            ++dp[row0][row1];
        }
    }

    /* add remaining rows */
    for (idx_t r = 2; r < n; ++r) {
        memset(ndp, 0, sizeof(ndp));
        for (int prev = 0; prev < 64; ++prev)
            for (int curr = 0; curr < 64; ++curr) {
                seq_t ways = dp[prev][curr];
                if (!ways) continue;
                uint64_t mask = trans_mask[prev][curr];
                while (mask) {
                    int next = __builtin_ctzll(mask);
                    mask &= mask - 1;
                    ndp[curr][next] += ways;
                }
            }
        memcpy(dp, ndp, sizeof(dp));
    }

    /* final row: enforce its own constraint (no lower neighbour) */
    seq_t result = 0;
    for (int prev = 0; prev < 64; ++prev)
        for (int curr = 0; curr < 64; ++curr)
            if (last_valid[prev][curr])
                result += dp[prev][curr];

    return result;
}
