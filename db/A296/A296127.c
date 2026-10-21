/*
 * A296127  Number of nX7 0..1 arrays with each 1 adjacent to 2 or 5 king-move
 *          neighboring 1s.
 * https://oeis.org/A296127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A296127: Number of n×7 binary arrays in which every 1 has 
 * exactly 2 or 5 king‑move neighboring 1s.
 * Computation: DP over rows.  The state is a pair of consecutive rows.
 * Transition: from (prevprev, prev) to (prev, cur) is allowed iff 
 *   every 1 in `prev` has a total of 2 or 5 neighboring 1s among 
 *   (prevprev, prev, cur).  For the first row the fictitious previous
 *   row is all 0s; after processing all rows we validate the last row
 *   against a fictitious following row of all 0s.
 */

#include <stdint.h>
#include <stdbool.h>

struct trans_t {
    uint8_t cnt;
    uint8_t next[128];
};

static struct trans_t trans[128][128];
static bool trans_ready = false;

static bool valid_row(uint8_t row, uint8_t above, uint8_t below)
{
    for (int j = 0; j < 7; ++j) {
        if (!(row & (1u << j))) continue;
        int cnt = 0;
        /* horizontal neighbors */
        if (j > 0  && (row   & (1u << (j - 1)))) ++cnt;
        if (j < 6  && (row   & (1u << (j + 1)))) ++cnt;
        /* vertical neighbors from above */
        if (above & (1u << j)) ++cnt;
        if (j > 0  && (above & (1u << (j - 1)))) ++cnt;
        if (j < 6  && (above & (1u << (j + 1)))) ++cnt;
        /* vertical neighbors from below */
        if (below & (1u << j)) ++cnt;
        if (j > 0  && (below & (1u << (j - 1)))) ++cnt;
        if (j < 6  && (below & (1u << (j + 1)))) ++cnt;
        if (cnt != 2 && cnt != 5) return false;
    }
    return true;
}

static void build_transitions(void)
{
    if (trans_ready) return;
    for (int a = 0; a < 128; ++a) {
        for (int b = 0; b < 128; ++b) {
            struct trans_t *t = &trans[a][b];
            t->cnt = 0;
            for (int q = 0; q < 128; ++q) {
                if (valid_row((uint8_t)b, (uint8_t)a, (uint8_t)q))
                    t->next[t->cnt++] = (uint8_t)q;
            }
        }
    }
    trans_ready = true;
}

#define STATE (128 * 128)

static seq_t dp_cur_arr[STATE];
static seq_t dp_next_arr[STATE];

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    build_transitions();

    seq_t *dp_cur = dp_cur_arr;
    seq_t *dp_next = dp_next_arr;

    /* initialise dp_cur for the first row (prevprev = 0) */
    for (idx_t i = 0; i < STATE; ++i) dp_cur[i] = (seq_t)0;
    for (uint8_t p = 0; p < 128; ++p) dp_cur[p] = (seq_t)1;   /* state (0,p) */

    /* process rows 2 … n */
    for (idx_t row = 2; row <= n; ++row) {
        for (idx_t i = 0; i < STATE; ++i) dp_next[i] = (seq_t)0;

        for (idx_t s = 0; s < STATE; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            uint8_t a = s >> 7;
            uint8_t b = s & 0x7Fu;
            const struct trans_t *t = &trans[a][b];
            for (uint8_t k = 0; k < t->cnt; ++k) {
                uint8_t q = t->next[k];
                idx_t ns = ((idx_t)b << 7) | q;
                dp_next[ns] += val;
            }
        }
        /* swap tables */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* final dummy row (below is 0) – validate last real row and sum */
    seq_t ans = (seq_t)0;
    for (idx_t s = 0; s < STATE; ++s) {
        seq_t val = dp_cur[s];
        if (val == (seq_t)0) continue;
        uint8_t a = s >> 7;
        uint8_t b = s & 0x7Fu;
        if (valid_row((uint8_t)b, (uint8_t)a, 0u))
            ans += val;
    }
    return ans;
}
