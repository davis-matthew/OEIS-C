/*
 * A299079  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 3,
 *          4 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A299079: Number of n×6 0/1 arrays with king‑move adjacency counts {0,1,2,3,4,7},
   upper left element zero.
   Computed by dynamic programming over 6‑bit row states. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precompute validity tables once. */
    static bool init = false;
    static bool top_ok[64][64];
    static bool bottom_ok[64][64];
    static bool middle_ok[64][64][64];
    static bool row_ok[64];

    /* allowed = {0,1,2,3,4,7} */
    const bool allowed[9] = {true,true,true,true,true,
                             false,false,true,false};

    if (!init) {
        /* row_ok: single row left/right adjacency only */
        for (uint64_t r = 0; r < 64; ++r) {
            bool ok = true;
            for (int c = 0; c < 6 && ok; ++c) {
                int val = (r >> c) & 1;
                int cnt = 0;
                if (c > 0 && ((r >> (c-1)) & 1) == val) ++cnt;
                if (c < 5 && ((r >> (c+1)) & 1) == val) ++cnt;
                if (!allowed[cnt]) ok = false;
            }
            row_ok[r] = ok;
        }

        /* top_ok: first row with second row (no above row) */
        for (uint64_t r0 = 0; r0 < 64; ++r0) {
            for (uint64_t r1 = 0; r1 < 64; ++r1) {
                bool ok = true;
                for (int c = 0; c < 6 && ok; ++c) {
                    int val = (r0 >> c) & 1;
                    int cnt = 0;
                    /* left/right in r0 */
                    if (c > 0 && ((r0 >> (c-1)) & 1) == val) ++cnt;
                    if (c < 5 && ((r0 >> (c+1)) & 1) == val) ++cnt;
                    /* below-left/right/below in r1 */
                    if (c > 0 && ((r1 >> (c-1)) & 1) == val) ++cnt;
                    if (((r1 >> c) & 1) == val) ++cnt;
                    if (c < 5 && ((r1 >> (c+1)) & 1) == val) ++cnt;
                    if (!allowed[cnt]) ok = false;
                }
                top_ok[r0][r1] = ok;
            }
        }

        /* bottom_ok: last row with previous row (no below row) */
        for (uint64_t prev = 0; prev < 64; ++prev) {
            for (uint64_t curr = 0; curr < 64; ++curr) {
                bool ok = true;
                for (int c = 0; c < 6 && ok; ++c) {
                    int val = (curr >> c) & 1;
                    int cnt = 0;
                    /* above-left/right/up in prev */
                    if (c > 0 && ((prev >> (c-1)) & 1) == val) ++cnt;
                    if (((prev >> c) & 1) == val) ++cnt;
                    if (c < 5 && ((prev >> (c+1)) & 1) == val) ++cnt;
                    /* left/right in curr */
                    if (c > 0 && ((curr >> (c-1)) & 1) == val) ++cnt;
                    if (c < 5 && ((curr >> (c+1)) & 1) == val) ++cnt;
                    if (!allowed[cnt]) ok = false;
                }
                bottom_ok[prev][curr] = ok;
            }
        }

        /* middle_ok: interior row with rows above and below */
        for (uint64_t up = 0; up < 64; ++up) {
            for (uint64_t curr = 0; curr < 64; ++curr) {
                for (uint64_t down = 0; down < 64; ++down) {
                    bool ok = true;
                    for (int c = 0; c < 6 && ok; ++c) {
                        int val = (curr >> c) & 1;
                        int cnt = 0;
                        /* above-left/right/up in up */
                        if (c > 0 && ((up >> (c-1)) & 1) == val) ++cnt;
                        if (((up >> c) & 1) == val) ++cnt;
                        if (c < 5 && ((up >> (c+1)) & 1) == val) ++cnt;
                        /* left/right in curr */
                        if (c > 0 && ((curr >> (c-1)) & 1) == val) ++cnt;
                        if (c < 5 && ((curr >> (c+1)) & 1) == val) ++cnt;
                        /* down-left/right/below in down */
                        if (c > 0 && ((down >> (c-1)) & 1) == val) ++cnt;
                        if (((down >> c) & 1) == val) ++cnt;
                        if (c < 5 && ((down >> (c+1)) & 1) == val) ++cnt;
                        if (!allowed[cnt]) ok = false;
                    }
                    middle_ok[up][curr][down] = ok;
                }
            }
        }
        init = true;
    }

    if (n == 0) return (seq_t)0;  /* out of offset */

    if (n == 1) {
        seq_t res = (seq_t)0;
        for (uint64_t r = 0; r < 64; ++r)
            if (!(r & 1) && row_ok[r]) ++res;
        return res;
    }

    /* DP arrays: number of ways to reach a pair (prev, curr) */
    static seq_t dp_curr[64][64];
    static seq_t dp_next[64][64];

    /* clear dp_curr */
    for (uint64_t i = 0; i < 64; ++i)
        for (uint64_t j = 0; j < 64; ++j)
            dp_curr[i][j] = (seq_t)0;

    /* initialize first two rows */
    for (uint64_t r0 = 0; r0 < 64; ++r0) {
        if (r0 & 1) continue;                 /* upper‑left = 0 */
        for (uint64_t r1 = 0; r1 < 64; ++r1) {
            if (top_ok[r0][r1]) dp_curr[r0][r1] = (seq_t)1;
        }
    }

    /* process remaining rows (n-2 of them) */
    for (idx_t step = 0; step < n - 2; ++step) {
        /* zero dp_next */
        for (uint64_t i = 0; i < 64; ++i)
            for (uint64_t j = 0; j < 64; ++j)
                dp_next[i][j] = (seq_t)0;

        for (uint64_t prev = 0; prev < 64; ++prev) {
            for (uint64_t curr = 0; curr < 64; ++curr) {
                seq_t val = dp_curr[prev][curr];
                if (val == (seq_t)0) continue;
                for (uint64_t next = 0; next < 64; ++next) {
                    if (middle_ok[prev][curr][next]) {
                        dp_next[curr][next] += val;
                    }
                }
            }
        }
        /* swap tables */
        for (uint64_t i = 0; i < 64; ++i)
            for (uint64_t j = 0; j < 64; ++j)
                dp_curr[i][j] = dp_next[i][j];
    }

    /* sum states satisfying bottom constraints */
    seq_t result = (seq_t)0;
    for (uint64_t prev = 0; prev < 64; ++prev) {
        for (uint64_t curr = 0; curr < 64; ++curr) {
            if (bottom_ok[prev][curr]) result += dp_curr[prev][curr];
        }
    }
    return result;
}
