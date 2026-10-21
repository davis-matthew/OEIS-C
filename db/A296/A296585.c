/*
 * A296585  Number of n X 5 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 0, 2 or 3 neighboring 1's.
 * https://oeis.org/A296585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of n×5 binary arrays in which each 1 cell has
 *            0, 2, or 3 neighbors among left, right, up, down, up‑left, and
 *            down‑right. This implementation uses a transfer‑matrix dynamic
 *            programming over row patterns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline bool valid(uint32_t prev, uint32_t curr, uint32_t next)
{
    for (int c = 0; c < 5; ++c) {
        if (curr & (1u << c)) {
            int cnt = 0;
            /* horizontal */
            if (c > 0 && (curr & (1u << (c - 1)))) ++cnt;
            if (c < 4 && (curr & (1u << (c + 1)))) ++cnt;
            /* vertical up */
            if (prev & (1u << c)) ++cnt;
            /* vertical up‑left */
            if (c > 0 && (prev & (1u << (c - 1)))) ++cnt;
            /* vertical down */
            if (next & (1u << c)) ++cnt;
            /* vertical down‑right */
            if (c < 4 && (next & (1u << (c + 1)))) ++cnt;
            if (cnt != 0 && cnt != 2 && cnt != 3)
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    const uint32_t PAT = 1u << 5;              /* 32 patterns for a row */
    const size_t ST = PAT * PAT;               /* states = prev × curr, 0‑based */

    /* Precompute compatibility mask for each (prev, curr) pair. */
    static uint32_t next_mask[32][32];
    static bool initialized = false;
    if (!initialized) {
        for (uint32_t prev = 0; prev < PAT; ++prev) {
            for (uint32_t curr = 0; curr < PAT; ++curr) {
                uint32_t m = 0;
                for (uint32_t next = 0; next < PAT; ++next) {
                    if (valid(prev, curr, next))
                        m |= 1u << next;
                }
                next_mask[prev][curr] = m;
            }
        }
        initialized = true;
    }

    seq_t dp_cur[ST];
    seq_t dp_next[ST];

    /* Initialise: first row with previous pattern 0. */
    for (size_t i = 0; i < ST; ++i) dp_cur[i] = 0;
    for (uint32_t r = 0; r < PAT; ++r)
        dp_cur[r] = 1;                /* state (prev=0, curr=r) */

    /* Process rows 2 .. n. */
    for (idx_t row = 2; row <= n; ++row) {
        for (size_t i = 0; i < ST; ++i) dp_next[i] = 0;
        for (uint32_t s = 0; s < ST; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            uint32_t prev = s >> 5;           /* s / PAT */
            uint32_t curr = s & 31;           /* s % PAT */
            uint32_t mask = next_mask[prev][curr];
            while (mask) {
                unsigned int next = __builtin_ctz(mask);
                dp_next[(curr << 5) | next] += val;
                mask &= mask - 1;
            }
        }
        /* swap dp_cur and dp_next */
        for (size_t i = 0; i < ST; ++i) dp_cur[i] = dp_next[i];
    }

    /* Final row: must be valid with a trailing zero row. */
    seq_t ans = 0;
    for (uint32_t s = 0; s < ST; ++s) {
        seq_t val = dp_cur[s];
        if (!val) continue;
        uint32_t prev = s >> 5;
        uint32_t curr = s & 31;
        if (next_mask[prev][curr] & 1u)    /* next pattern = 0 */
            ans += val;
    }
    return ans;
}
