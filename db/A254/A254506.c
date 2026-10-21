/*
 * A254506  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock sum of
 *          four medians of central row, central column, diagonal and
 *          antidiagonal nondecreasing horizontally, vertically and ne-to-sw
 *          antidiagonally.
 * https://oeis.org/A254506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+2)×4 0..1 arrays such that every 3×3 subblock
 *            satisfies the nondecreasing condition on the sum of the four
 *            medians (central row, central column, diagonal, antidiagonal)
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 55,
 *            big any n
 * Helpers:   _majority3, _state_decode, _state_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------   Includes and type safety   ------- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/_majority3.h"
#include "../utils/_state_decode.h"
#include "../utils/_state_index.h"

static bool _a254506_precomp_done = false;
/* mapping from (s0,s1), 0<=s0<=s1<=4  -> index 0..14 */
static int8_t _a254506_encode[5][5];
/* reverse mapping from index to s0,s1 if needed */
static uint8_t _a254506_decode_s0[15];
static uint8_t _a254506_decode_s1[15];

/* 3‑row block sums: 4‑bit rows → 0..4 each */
static uint8_t _a254506_block_s0[16][16][16];
static uint8_t _a254506_block_s1[16][16][16];

/* Transition table:
   [prev_row][cur_row][prev_sum_idx][new_row] = next_sum_idx (0..14) or -1 if invalid
   Flattened to 3840×16 */
static int8_t _a254506_next[3840][16];

/* Precompute block sums and transition table */
static void _a254506_precompute(void)
{
    if (_a254506_precomp_done)
        return;

    /* encode map */
    int idx = 0;
    for (int i = 0; i <= 4; ++i)
        for (int j = i; j <= 4; ++j)
            _a254506_encode[i][j] = idx++;

    for (int i = 0; i < 15; ++i) {
        for (int s0 = 0; s0 <= 4; ++s0)
            for (int s1 = s0; s1 <= 4; ++s1)
                if (_a254506_encode[s0][s1] == i) {
                    _a254506_decode_s0[i] = (uint8_t)s0;
                    _a254506_decode_s1[i] = (uint8_t)s1;
                }
    }

    /* block sums */
    for (uint8_t p = 0; p < 16; ++p)
        for (uint8_t q = 0; q < 16; ++q)
            for (uint8_t r = 0; r < 16; ++r) {
                uint8_t s0, s1;
                /* j = 0 */
                uint8_t m0 = _majority3((q>>0)&1, (q>>1)&1, (q>>2)&1);
                uint8_t m1 = _majority3((p>>1)&1, (q>>1)&1, (r>>1)&1);
                uint8_t m2 = _majority3((p>>0)&1, (q>>1)&1, (r>>2)&1);
                uint8_t m3 = _majority3((p>>2)&1, (q>>1)&1, (r>>0)&1);
                s0 = m0 + m1 + m2 + m3;
                /* j = 1 */
                uint8_t m4 = _majority3((q>>1)&1, (q>>2)&1, (q>>3)&1);
                uint8_t m5 = _majority3((p>>2)&1, (q>>2)&1, (r>>2)&1);
                uint8_t m6 = _majority3((p>>1)&1, (q>>2)&1, (r>>3)&1);
                uint8_t m7 = _majority3((p>>3)&1, (q>>2)&1, (r>>1)&1);
                s1 = m4 + m5 + m6 + m7;
                _a254506_block_s0[p][q][r] = s0;
                _a254506_block_s1[p][q][r] = s1;
            }

    /* transition table */
    for (uint8_t prev = 0; prev < 16; ++prev)
        for (uint8_t cur = 0; cur < 16; ++cur)
            for (uint8_t sum_idx = 0; sum_idx < 15; ++sum_idx) {
                size_t state = _state_index(prev, cur, sum_idx);
                uint8_t prev0 = _a254506_decode_s0[sum_idx];
                uint8_t prev1 = _a254506_decode_s1[sum_idx];
                for (uint8_t nxt = 0; nxt < 16; ++nxt) {
                    /* compute sums for block (prev,cur,nxt) */
                    uint8_t s0 = _a254506_block_s0[prev][cur][nxt];
                    uint8_t s1 = _a254506_block_s1[prev][cur][nxt];
                    /* constraints */
                    if (prev0 <= s0 && prev1 <= s1 &&
                        prev1 <= s0 && s0 <= s1) {
                        int8_t nxt_idx = _a254506_encode[s0][s1];
                        _a254506_next[state][nxt] = nxt_idx;
                    } else {
                        _a254506_next[state][nxt] = -1;
                    }
                }
            }

    _a254506_precomp_done = true;
}

/* Main function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* out of offset, but defined as 0 */

    _a254506_precompute();

    /* DP arrays */
    static seq_t dp_cur[3840];
    static seq_t dp_next[3840];

    /* clear dp_cur */
    for (size_t i = 0; i < 3840; ++i)
        dp_cur[i] = 0;

    /* initial states: first 3 rows a,b,c */
    for (uint8_t a = 0; a < 16; ++a)
        for (uint8_t b = 0; b < 16; ++b)
            for (uint8_t c = 0; c < 16; ++c) {
                uint8_t s0 = _a254506_block_s0[a][b][c];
                uint8_t s1 = _a254506_block_s1[a][b][c];
                if (s0 <= s1) {
                    uint8_t sum_idx = _a254506_encode[s0][s1];
                    size_t state = _state_index(b, c, sum_idx);
                    dp_cur[state] += 1;
                }
            }

    /* Remaining rows (there are n+2 rows total, we already used 3) */
    idx_t steps = n - 1; /* number of further rows to add */
    for (idx_t step = 0; step < steps; ++step) {
        /* clear dp_next */
        for (size_t i = 0; i < 3840; ++i)
            dp_next[i] = 0;

        for (size_t state = 0; state < 3840; ++state) {
            seq_t cnt = dp_cur[state];
            if (!cnt)
                continue;
            uint8_t prev = (state / 15) >> 4;
            uint8_t cur  = (state / 15) & 0xF;
            uint8_t sum_idx = state % 15;
            /* try all possible new rows */
            for (uint8_t nxt = 0; nxt < 16; ++nxt) {
                int8_t nxt_sum = _a254506_next[state][nxt];
                if (nxt_sum < 0)
                    continue;
                size_t new_state = _state_index(cur, nxt, (uint8_t)nxt_sum);
                dp_next[new_state] += cnt;
            }
        }
        /* swap */
        for (size_t i = 0; i < 3840; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all counts in final state array */
    seq_t ans = 0;
    for (size_t i = 0; i < 3840; ++i)
        ans += dp_cur[i];

    return ans;
}
