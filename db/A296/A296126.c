/*
 * A296126  Number of nX6 0..1 arrays with each 1 adjacent to 2 or 5 king-move
 *          neighboring 1s.
 * https://oeis.org/A296126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = Number of n×6 binary arrays in which every 1 has exactly 2
 * or 5 king-move neighboring 1s.  Computed by dynamic programming
 * over rows with a sliding window of three rows.
 */

#include <stdint.h>
#include <stdbool.h>

/* Pre‑computation table: for each pair (prev,curr) the bitmask of
 * next rows that keep curr valid. */
static uint64_t next_valid[64][64];

/* Helper that checks whether the row 'curr' satisfies the adjacency
 * condition when it has neighbours 'prev' above and 'next' below. */
static inline bool valid_middle(uint8_t prev, uint8_t curr, uint8_t next)
{
    for (int j = 0; j < 6; ++j) {
        if (!(curr & (1u << j))) continue;          /* only cells containing 1 */
        int cnt = 0;
        /* above row */
        if (j > 0   && (prev & (1u << (j - 1)))) ++cnt;
        if (prev & (1u << j))                      ++cnt;
        if (j < 5   && (prev & (1u << (j + 1)))) ++cnt;
        /* current row (horizontal neighbours) */
        if (j > 0   && (curr & (1u << (j - 1)))) ++cnt;
        if (j < 5   && (curr & (1u << (j + 1)))) ++cnt;
        /* below row */
        if (j > 0   && (next & (1u << (j - 1)))) ++cnt;
        if (next & (1u << j))                     ++cnt;
        if (j < 5   && (next & (1u << (j + 1)))) ++cnt;
        if (!(cnt == 2 || cnt == 5)) return false;
    }
    return true;
}

/* Build the transition masks once. */
static void precompute_next_valid(void)
{
    static bool built = false;
    if (built) return;

    for (uint8_t prev = 0; prev < 64; ++prev) {
        for (uint8_t curr = 0; curr < 64; ++curr) {
            uint64_t mask = 0;
            for (uint8_t next = 0; next < 64; ++next) {
                if (valid_middle(prev, curr, next))
                    mask |= 1ULL << next;
            }
            next_valid[prev][curr] = mask;
        }
    }
    built = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* size 0 matrix does not concern the OEIS offset */

    precompute_next_valid();

    /* dp[prev][curr] = number of ways to reach current configuration
     * where the last two rows are 'prev' (above) and 'curr' (current). */
    seq_t dp[64][64] = {0};
    seq_t ndp[64][64];

    dp[0][0] = (seq_t)1;                 /* start with two dummy zero rows */

    for (idx_t step = 0; step < n; ++step) {
        /* zero ndp by element (not memcpy, safe for non‑POD seq_t) */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                ndp[i][j] = (seq_t)0;

        for (uint8_t prev = 0; prev < 64; ++prev) {
            for (uint8_t curr = 0; curr < 64; ++curr) {
                seq_t val = dp[prev][curr];
                if (val == (seq_t)0) continue;

                uint64_t mask = next_valid[prev][curr];
                while (mask) {
                    uint8_t next = (uint8_t)__builtin_ctzll(mask);
                    ndp[curr][next] += val;
                    mask &= mask - 1;
                }
            }
        }

        /* copy ndp into dp for next iteration */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* final step: enforce that the last real row (curr) is valid with
     * a zero row below it (next = 0).  The bit 0 in the mask indicates
     * that next = 0 is acceptable. */
    seq_t answer = (seq_t)0;
    for (uint8_t prev = 0; prev < 64; ++prev) {
        for (uint8_t curr = 0; curr < 64; ++curr) {
            if (dp[prev][curr] == (seq_t)0) continue;
            if (next_valid[prev][curr] & 1ULL)   /* bit 0 set? */
                answer += dp[prev][curr];
        }
    }

    return answer;
}
