/*
 * A295649  Number of n X 5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0, 1, 3 or 4 1's.
 * https://oeis.org/A295649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Counts of n×5 binary arrays where each 1 has 0,1,3 or 4
 *            neighboring 1's (horizontal/vertical adjacency)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define COLS 5
#define STATE_BITS (COLS + 2 * COLS)   /* 5 mask bits + 10 bits for partial degrees */
#define STATE_COUNT (1 << STATE_BITS)  /* 32768 */

static int next_state[STATE_COUNT][32];
static uint8_t next_count[STATE_COUNT];
static bool transitions_initialized = false;

static void init_transitions(void)
{
    for (int s = 0; s < STATE_COUNT; ++s) {
        int mask = s & 0x1F;                 /* lower 5 bits: current row mask */
        uint8_t pvec[COLS];
        for (int j = 0; j < COLS; ++j)
            pvec[j] = (s >> (5 + 2 * j)) & 3;

        /* Determine allowed next bits for each column */
        int allow0[COLS], allow1[COLS];
        for (int j = 0; j < COLS; ++j) {
            if (!(mask & (1 << j))) {
                allow0[j] = 1;  allow1[j] = 1;   /* free column */
            } else {
                int p = pvec[j];
                allow0[j] = (p == 0 || p == 1 || p == 3);
                allow1[j] = ((p + 1) == 0 || (p + 1) == 1 || (p + 1) == 3 || (p + 1) == 4);
            }
        }

        int cnt = 0;
        for (int nmask = 0; nmask < 32; ++nmask) {
            bool ok = true;
            for (int j = 0; j < COLS && ok; ++j) {
                if (mask & (1 << j)) {
                    int bit = (nmask >> j) & 1;
                    if (bit == 0 && !allow0[j]) ok = false;
                    if (bit == 1 && !allow1[j]) ok = false;
                }
            }
            if (!ok) continue;

            uint8_t newp[COLS] = {0};
            for (int j = 0; j < COLS; ++j) {
                if (nmask & (1 << j)) {
                    int left  = (j > 0)     ? ((nmask >> (j - 1)) & 1) : 0;
                    int right = (j < COLS-1) ? ((nmask >> (j + 1)) & 1) : 0;
                    int horiz = left + right;
                    int up    = (mask >> j) & 1;
                    newp[j] = horiz + up;   /* 0..3 */
                }
            }

            int ns = nmask;
            for (int j = 0; j < COLS; ++j)
                ns |= (int)newp[j] << (5 + 2 * j);

            next_state[s][cnt++] = ns;
        }
        next_count[s] = (uint8_t)cnt;
    }
    transitions_initialized = true;
}

seq_t a(idx_t n)
{
    if (!transitions_initialized)
        init_transitions();

    static seq_t dp0[STATE_COUNT];
    static seq_t dp1[STATE_COUNT];
    seq_t *dp_cur = dp0, *dp_next = dp1;

    /* initialise dp_cur to zero */
    for (int i = 0; i < STATE_COUNT; ++i)
        dp_cur[i] = 0;

    /* first row initialization */
    for (int mask = 0; mask < 32; ++mask) {
        uint8_t p[COLS] = {0};
        for (int j = 0; j < COLS; ++j) {
            if (mask & (1 << j)) {
                int left  = (j > 0)     ? ((mask >> (j - 1)) & 1) : 0;
                int right = (j < COLS-1) ? ((mask >> (j + 1)) & 1) : 0;
                p[j] = left + right;   /* up neighbour is zero */
            }
        }
        int st = mask;
        for (int j = 0; j < COLS; ++j)
            st |= (int)p[j] << (5 + 2 * j);
        dp_cur[st] = 1;
    }

    /* rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < STATE_COUNT; ++i)
            dp_next[i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int c = next_count[s];
            for (int k = 0; k < c; ++k) {
                int ns = next_state[s][k];
                dp_next[ns] += val;
            }
        }
        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* final filtering: last row cells must satisfy p != 2 (degree 2 is illegal with no down neighbour) */
    seq_t ans = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        seq_t val = dp_cur[s];
        if (val == 0) continue;
        int mask = s & 0x1F;
        bool ok = true;
        for (int j = 0; j < COLS && ok; ++j) {
            if (mask & (1 << j)) {
                int p = (s >> (5 + 2 * j)) & 3;
                if (p == 2) ok = false;
            }
        }
        if (ok) ans += val;
    }
    return ans;
}
