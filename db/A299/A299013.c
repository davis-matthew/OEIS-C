/*
 * A299013  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 4,
 *          5 or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with * each element equal to
 *            the count of same‑valued king‑move * adjacent elements, 3, 7 or
 *            8 are forbidden, upper‑left cell * is zero. Computed by
 *            transfer‑matrix DP
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool initialized = false;
static unsigned long long trans_mask[64][64];
static unsigned char trans_len[64][64];
static unsigned long long no_above_mask[64];
static unsigned char no_above_len[64];

/* Check validity of a row, given its upper and lower neighbour rows.
 * hasPrev / hasNext indicate whether the neighbour row actually exists. */
static bool row_ok(uint8_t prev, bool hasPrev, uint8_t curr,
                   bool hasNext, uint8_t next)
{
    for (int j = 0; j < 6; ++j) {
        int cell = (curr >> j) & 1;
        int match = 0;
        /* same row neighbours */
        if (j > 0 && ((curr >> (j - 1)) & 1) == cell) ++match;
        if (j < 5 && ((curr >> (j + 1)) & 1) == cell) ++match;
        /* upper neighbours */
        if (hasPrev) {
            if (j > 0 && ((prev >> (j - 1)) & 1) == cell) ++match;
            if (((prev >> j) & 1) == cell) ++match;
            if (j < 5 && ((prev >> (j + 1)) & 1) == cell) ++match;
        }
        /* lower neighbours */
        if (hasNext) {
            if (j > 0 && ((next >> (j - 1)) & 1) == cell) ++match;
            if (((next >> j) & 1) == cell) ++match;
            if (j < 5 && ((next >> (j + 1)) & 1) == cell) ++match;
        }
        if (match == 3 || match == 7 || match == 8) return false;
    }
    return true;
}

/* Pre‑compute transition masks once. */
static void precompute_masks(void)
{
    if (initialized) return;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            unsigned long long mask = 0;
            for (int nxt = 0; nxt < 64; ++nxt) {
                if (row_ok((uint8_t)prev, true, (uint8_t)curr, true,
                           (uint8_t)nxt))
                    mask |= 1ULL << nxt;
            }
            trans_mask[prev][curr] = mask;
            trans_len[prev][curr]  = (unsigned char)__builtin_popcountll(mask);
        }
        /* no‑above table: row `prev` is the lower row, upper row absent */
        unsigned long long mask = 0;
        for (int nxt = 0; nxt < 64; ++nxt) {
            if (row_ok(0, false, (uint8_t)prev, true, (uint8_t)nxt))
                mask |= 1ULL << nxt;
        }
        no_above_mask[prev] = mask;
        no_above_len[prev]  = (unsigned char)__builtin_popcountll(mask);
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    precompute_masks();

    /* Special case n==1: only first row, no neighbours above or below */
    if (n == 1) {
        seq_t result = 0;
        for (int r = 0; r < 64; ++r) {
            if ((r & 1) == 0)        /* upper‑left zero */
                if (row_ok(0, false, (uint8_t)r, false, 0))
                    result += (seq_t)1;
        }
        return result;
    }

    /* DP arrays: current and next state */
    static seq_t dp0[64][64], dp1[64][64];
    seq_t (*cur)[64]  = dp0;
    seq_t (*next)[64] = dp1;

    /* initialise all entries to zero */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            cur[i][j] = next[i][j] = 0;

    /* Initialisation: set first two rows */
    for (int r0 = 0; r0 < 64; ++r0) {
        if ((r0 & 1) != 0) continue;           /* upper‑left zero */
        unsigned long long mask = no_above_mask[r0];
        while (mask) {
            unsigned long long t = mask & -mask;
            int r1 = __builtin_ctzll(mask);
            cur[r0][r1] += (seq_t)1;
            mask -= t;
        }
    }

    /* Process intermediate rows 2 .. n‑1 */
    for (idx_t step = 2; step <= n - 1; ++step) {
        /* zero next array */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                next[i][j] = 0;
        /* transition */
        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t val = cur[prev][curr];
                if (!val) continue;
                unsigned long long mask = trans_mask[prev][curr];
                while (mask) {
                    unsigned long long t = mask & -mask;
                    int nxt = __builtin_ctzll(mask);
                    next[curr][nxt] += val;
                    mask -= t;
                }
            }
        }
        /* swap pointers */
        seq_t (*tmp)[64] = cur;
        cur = next;
        next = tmp;
    }

    /* Final row evaluation (last row has no neighbour below) */
    seq_t result = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            seq_t val = cur[prev][curr];
            if (!val) continue;
            if (row_ok((uint8_t)prev, true, (uint8_t)curr, false, 0))
                result += val;
        }
    }

    return result;
}
