/*
 * A296649  Number of nX6 0..1 arrays with each 1 adjacent to 0, 1 or 3
 *          king-move neighboring 1s.
 * https://oeis.org/A296649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 194 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP over rows for n×6 0..1 arrays where every 1 has 0,1, or 3
 * king‑move neighbouring 1s.  The DP keeps track of two consecutive rows and
 * precomputes, for each (row‑above, current row) pair, the set of next rows
 * that keep all 1s in the current row valid.  After processing all rows a
 * final check is performed on the last row against a non‑existent bottom
 * neighbour. */
#include <stdint.h>
#include <stddef.h>

/* Helper: check if all 1s in 'prev' are valid when the row above is 'prevPrev'
 * and the row below is 'curr'.  The adjacencies are counted with king moves.
 * Returns 1 if valid, 0 otherwise. */
static int
is_valid_rows(uint64_t prevPrev, uint64_t prev, uint64_t curr)
{
    /* iterate over all columns j=0..5 */
    for (int j = 0; j < 6; ++j) {
        if (!((prev >> j) & 1ULL))
            continue;          /* nothing to check */
        int count = 0;
        /* horizontal neighbors in the same row */
        if (j > 0 && ((prev >> (j - 1)) & 1ULL)) count++;
        if (j + 1 < 6 && ((prev >> (j + 1)) & 1ULL)) count++;
        /* vertical and diagonal neighbors from the row above */
        if ((prevPrev >> j) & 1ULL) count++;                           /* up */
        if (j > 0 && ((prevPrev >> (j - 1)) & 1ULL)) count++;          /* up‑left */
        if (j + 1 < 6 && ((prevPrev >> (j + 1)) & 1ULL)) count++;      /* up‑right */
        /* vertical and diagonal neighbors from the row below */
        if ((curr >> j) & 1ULL) count++;                             /* down */
        if (j > 0 && ((curr >> (j - 1)) & 1ULL)) count++;              /* down‑left */
        if (j + 1 < 6 && ((curr >> (j + 1)) & 1ULL)) count++;          /* down‑right */
        /* allowed totals are 0, 1 or 3 */
        if (count != 0 && count != 1 && count != 3)
            return 0;
    }
    return 1;
}

/* Precomputation for UI: next masks and final validity check. */
static void
precompute_masks(uint64_t nextMask[4096], unsigned char finalGood[4096])
{
    for (uint64_t prevPrev = 0; prevPrev < 64; ++prevPrev) {
        for (uint64_t prev = 0; prev < 64; ++prev) {
            uint64_t mask = 0;
            for (uint64_t curr = 0; curr < 64; ++curr) {
                if (is_valid_rows(prevPrev, prev, curr))
                    mask |= 1ULL << curr;
            }
            nextMask[prevPrev * 64 + prev] = mask;
            finalGood[prevPrev * 64 + prev] = (uint64_t)is_valid_rows(prevPrev, prev, 0);
        }
    }
}

/* Forward declare the helper that is provided by the framework.  All
 * seq_t / idx_t types are defined externally. */
static seq_t *alloc_seq(size_t);

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* offset is 1, but guard against 0 */

    /* Static tables, computed once */
    static uint64_t nextMask[4096];
    static unsigned char finalGood[4096];
    static int ready = 0;
    if (!ready) {
        precompute_masks(nextMask, finalGood);
        ready = 1;
    }

    /* DP arrays: flat index = prevPrev*64 + prev */
    seq_t dp[4096];
    seq_t newdp[4096];

    /* initialise dp: first row, above row is all zeros (0) */
    for (int i = 0; i < 4096; ++i) dp[i] = 0;
    for (int p = 0; p < 64; ++p) dp[p] = 1;      /* index = 0*64 + p = p */

    /* iterate over remaining rows 1 ... n-1 */
    for (idx_t row = 1; row < n; ++row) {
        for (int i = 0; i < 4096; ++i) newdp[i] = 0;
        for (uint64_t prevPrev = 0; prevPrev < 64; ++prevPrev) {
            for (uint64_t prev = 0; prev < 64; ++prev) {
                seq_t val = dp[prevPrev * 64 + prev];
                if (val == 0) continue;
                uint64_t mask = nextMask[prevPrev * 64 + prev];
                uint64_t m = mask;
                while (m) {
                    int curr = __builtin_ctzll(m);
                    newdp[prev * 64 + curr] += val;
                    m &= m - 1;
                }
            }
        }
        /* copy newdp into dp for next iteration */
        for (int i = 0; i < 4096; ++i)
            dp[i] = newdp[i];
    }

    /* final check on the last row */
    seq_t total = 0;
    for (uint64_t prevPrev = 0; prevPrev < 64; ++prevPrev) {
        for (uint64_t prev = 0; prev < 64; ++prev) {
            if (finalGood[prevPrev * 64 + prev])
                total += dp[prevPrev * 64 + prev];
        }
    }
    return total;
}
