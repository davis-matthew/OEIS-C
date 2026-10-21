/*
 * A297603  Number of nX4 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 1 or 3 neighboring 1s.
 * https://oeis.org/A297603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays with the property that each 1
 *            has 0, 1, or 3 horizontally‑adjacent or diagonally (including
 *            antidiagonal) neighbours. Computed by a dynamic‑programming scan
 *            over the rows. No closed form is available
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ---- helpers for state encoding ----------------------------------- */
static inline uint16_t encode_state(uint8_t mask,
                                    uint8_t p0, uint8_t p1,
                                    uint8_t p2, uint8_t p3)
{
    return (mask << 8) | (p0 << 6) | (p1 << 4) | (p2 << 2) | p3;
}

static inline void decode_state(uint16_t state,
                                uint8_t *mask,
                                uint8_t *p0, uint8_t *p1,
                                uint8_t *p2, uint8_t *p3)
{
    *mask = state >> 8;
    *p0   = (state >> 6) & 3;
    *p1   = (state >> 4) & 3;
    *p2   = (state >> 2) & 3;
    *p3   = state & 3;
}

/* ---- transition table --------------------------------------------- */
#define STATE_COUNT 4096          /* 2^12 possible states */
#define MAX_NEIGH   16            /* upper bound on transitions per state */

static uint8_t  state_mask[STATE_COUNT];
static uint8_t  state_partial[STATE_COUNT][4];
static uint16_t trans_next[STATE_COUNT][MAX_NEIGH];
static uint8_t  trans_cnt[STATE_COUNT];
static bool     trans_built = false;

/* Build transition matrix once, before any calls to a() */
static void build_transition_table(void)
{
    if (trans_built) return;

    /* initialise state data from the encoded states */
    for (uint16_t s = 0; s < STATE_COUNT; ++s) {
        uint8_t mask, p0, p1, p2, p3;
        decode_state(s, &mask, &p0, &p1, &p2, &p3);
        state_mask[s]   = mask;
        state_partial[s][0] = p0;
        state_partial[s][1] = p1;
        state_partial[s][2] = p2;
        state_partial[s][3] = p3;
    }

    /* compute transitions */
    for (uint16_t s = 0; s < STATE_COUNT; ++s) {
        uint8_t prev_mask = state_mask[s];
        uint8_t *pp = state_partial[s];
        uint8_t ncnt = 0;

        for (uint8_t new_mask = 0; new_mask < 16; ++new_mask) {
            bool ok = true;

            /* Validate previous row's 1s against the new row (down neighbors) */
            for (int c = 0; c < 4 && ok; ++c) {
                if ((prev_mask >> c) & 1) {
                    int down_l = (c > 0)   ? ((new_mask >> (c - 1)) & 1) : 0;
                    int down_r = (c < 3)   ? ((new_mask >> (c + 1)) & 1) : 0;
                    int final  = pp[c] + down_l + down_r;
                    if (final != 0 && final != 1 && final != 3) ok = false;
                }
            }
            if (!ok) continue;

            /* Compute partial counts for new row's 1s (left/right + up‑diagonals) */
            uint8_t p_new[4] = {0, 0, 0, 0};
            for (int c = 0; c < 4; ++c) {
                if ((new_mask >> c) & 1) {
                    int left   = (c > 0)   ? ((new_mask >> (c - 1)) & 1) : 0;
                    int right  = (c < 3)   ? ((new_mask >> (c + 1)) & 1) : 0;
                    int up_l   = (c > 0)   ? ((prev_mask >> (c - 1)) & 1) : 0;
                    int up_r   = (c < 3)   ? ((prev_mask >> (c + 1)) & 1) : 0;
                    int pc = left + right + up_l + up_r;
                    if (pc > 3) { ok = false; break; }
                    p_new[c] = (uint8_t)pc;
                }
            }
            if (!ok) continue;

            uint16_t next_state = encode_state(new_mask,
                                               p_new[0], p_new[1],
                                               p_new[2], p_new[3]);
            trans_next[s][ncnt++] = next_state;
        }
        trans_cnt[s] = ncnt;
    }
    trans_built = true;
}

/* ---- main function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    build_transition_table();

    /* DP buffers */
    static seq_t dp_cur[STATE_COUNT];
    static seq_t dp_next[STATE_COUNT];
    static seq_t *cur = NULL;
    static seq_t *next = NULL;
    if (cur == NULL) { cur = dp_cur; next = dp_next; }

    /* initialise with empty previous row */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 0;
    cur[0] = (seq_t)1;                /* state 0 is mask=0, partials=0 */

    /* walk through the rows */
    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t ways = cur[s];
            if (!ways) continue;
            uint8_t cnt = trans_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                uint16_t ns = trans_next[s][k];
                next[ns] += ways;
            }
        }
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* final validation for the last row */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        seq_t ways = cur[s];
        if (!ways) continue;

        uint8_t mask, p0, p1, p2, p3;
        decode_state((uint16_t)s, &mask, &p0, &p1, &p2, &p3);
        bool ok = true;
        if ((mask >> 0) & 1) {
            if (p0 != 0 && p0 != 1 && p0 != 3) ok = false;
        }
        if ((mask >> 1) & 1) {
            if (p1 != 0 && p1 != 1 && p1 != 3) ok = false;
        }
        if ((mask >> 2) & 1) {
            if (p2 != 0 && p2 != 1 && p2 != 3) ok = false;
        }
        if ((mask >> 3) & 1) {
            if (p3 != 0 && p3 != 1 && p3 != 3) ok = false;
        }
        if (!ok) continue;
        result += ways;
    }
    return result;
}
