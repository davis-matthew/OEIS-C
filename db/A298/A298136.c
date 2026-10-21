/*
 * A298136  Number of n X 5 0..1 arrays with every element equal to 2, 3, 5 or
 *          7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   _count_same_neighbors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A298136: number of n×5 binary arrays with every element
   equal to 2, 3, 5 or 7 king‑move adjacent elements, upper left element zero.
   Computed by dynamic programming over rows. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_count_same_neighbors.h"

static bool _mask_initialized = false;
static unsigned int _middleMask[32][32];
static unsigned int _topMask[32];
static bool _bottomValid[32][32];
static bool _row1Good[32];

static void _build_masks(void) {
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            unsigned mask = 0;
            for (int next = 0; next < 32; ++next) {
                if (_count_same_neighbors(true, (uint8_t)prev,
                                          (uint8_t)curr,
                                          true, (uint8_t)next))
                    mask |= 1u << next;
            }
            _middleMask[prev][curr] = mask;
        }
    }
    for (int curr = 0; curr < 32; ++curr) {
        unsigned mask = 0;
        for (int next = 0; next < 32; ++next) {
            if (_count_same_neighbors(false, 0,
                                      (uint8_t)curr,
                                      true, (uint8_t)next))
                mask |= 1u << next;
        }
        _topMask[curr] = mask;
    }
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            _bottomValid[prev][curr] =
                _count_same_neighbors(true, (uint8_t)prev,
                                      (uint8_t)curr,
                                      false, 0);
        }
    }
    for (int curr = 0; curr < 32; ++curr) {
        _row1Good[curr] =
            _count_same_neighbors(false, 0,
                                  (uint8_t)curr,
                                  false, 0);
    }
    _mask_initialized = true;
}

seq_t a(idx_t n) {
    if (!_mask_initialized) _build_masks();

    if (n == 0) return 0;                 /* not part of sequence */
    if (n == 1) {
        seq_t res = 0;
        for (int row = 0; row < 32; ++row) {
            if ((row & 1) == 0 && _row1Good[row]) {
                res += (seq_t)1;
            }
        }
        return res;
    }

    /* dp[prev][curr] : number of ways for rows up to current row,
       where prev = row i-1, curr = row i, and rows 1..i-1 satisfied. */
    seq_t dp[32][32] = {0};
    seq_t newdp[32][32] = {0};

    /* Initialization: row 1 (must have left‑most bit 0) and row 2 */
    for (int r1 = 0; r1 < 32; ++r1) {
        if ((r1 & 1) == 0) {                 /* upper‑left zero */
            unsigned mask = _topMask[r1];
            for (int r2 = 0; r2 < 32; ++r2) {
                if (mask & (1u << r2)) {
                    dp[r1][r2] = (seq_t)1;
                }
            }
        }
    }

    /* Process rows 3 .. n */
    for (idx_t idx = 2; idx < n; ++idx) {
        /* zero newdp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                newdp[i][j] = 0;

        for (int prev = 0; prev < 32; ++prev) {
            for (int curr = 0; curr < 32; ++curr) {
                seq_t ways = dp[prev][curr];
                if (ways == 0) continue;
                unsigned mask = _middleMask[prev][curr];
                for (int next = 0; next < 32; ++next) {
                    if (mask & (1u << next)) {
                        newdp[curr][next] += ways;
                    }
                }
            }
        }
        /* copy newdp to dp */
        for (int i = 0; i < 32; ++i)
            for (int j = 0; j < 32; ++j)
                dp[i][j] = newdp[i][j];
    }

    /* Sum over states where last row is valid without a following row */
    seq_t result = 0;
    for (int prev = 0; prev < 32; ++prev) {
        for (int curr = 0; curr < 32; ++curr) {
            if (dp[prev][curr] == 0) continue;
            if (_bottomValid[prev][curr])
                result += dp[prev][curr];
        }
    }

    return result;
}
