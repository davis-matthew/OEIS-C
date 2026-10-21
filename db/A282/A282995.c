/*
 * A282995  Number of nX7 0..1 arrays with no 1 equal to more than one of its
 *          horizontal and vertical neighbors.
 * https://oeis.org/A282995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with no 1 having more than *
 *            one neighbour among the horizontal and vertical adjacencies. *
 *            Computation by dynamic programming over the rows. For each row
 *            we * keep the current 7‑bit pattern and a 7‑bit mask that
 *            indicates * whether a cell in that pattern alread...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Width of the array */
#define PAT_BITS 7
#define PAT_COUNT (1U << PAT_BITS)               /* 128 */
#define STATE_COUNT (PAT_COUNT * PAT_COUNT)      /* 16384 */

/* Pre‑computed data */
static bool _initialized = false;
static uint8_t _allowed[PAT_COUNT];
static uint8_t _horiz_mask[PAT_COUNT];

/* Transitions: maximum 81 destinations per state */
static uint16_t _next_state[STATE_COUNT][81];
static uint8_t   _trans_count[STATE_COUNT];

/* Build the tables once */
static void _precompute(void)
{
    if (_initialized) return;
    _initialized = true;

    /* Determine which patterns have no 111 run and compute horizontal
       neighbour masks. */
    for (int pat = 0; pat < PAT_COUNT; ++pat) {
        _allowed[pat] = ((pat & (pat >> 1) & (pat >> 2)) == 0);
        /* bit j is 1 if the cell has a horizontal neighbour in this row */
        _horiz_mask[pat] = pat & (((pat << 1) | (pat >> 1)) & ((1 << PAT_BITS) - 1));
    }

    /* Enumerate all (prev_pat, used) states and compute transitions. */
    for (int prev_pat = 0; prev_pat < PAT_COUNT; ++prev_pat) {
        for (int used = 0; used < PAT_COUNT; ++used) {
            int sidx = (prev_pat << PAT_BITS) | used;   /* state index */
            /* used may only contain bits that are 1 in prev_pat */
            if ((used & ~prev_pat) != 0) { _trans_count[sidx] = 0; continue; }
            if (!_allowed[prev_pat]) { _trans_count[sidx] = 0; continue; }

            uint8_t cnt = 0;
            for (int cur_pat = 0; cur_pat < PAT_COUNT; ++cur_pat) {
                if (!_allowed[cur_pat]) continue;

                bool valid = true;

                /* ---- vertical adjacency ---- */
                for (int j = 0; j < PAT_BITS && valid; ++j) {
                    bool cur_bit = (cur_pat >> j) & 1;
                    bool prev_bit = (prev_pat >> j) & 1;
                    if (cur_bit && prev_bit) {
                        /* previous 1 must not already have a neighbour */
                        if ((used >> j) & 1) { valid = false; break; }
                        /* current 1 must not already have a horizontal neighbour */
                        bool left  = (j > 0)   && ((cur_pat >> (j - 1)) & 1);
                        bool right = (j < PAT_BITS-1) && ((cur_pat >> (j + 1)) & 1);
                        if (left || right) { valid = false; break; }
                    }
                }

                /* ---- horizontal adjacency in current row ---- */
                for (int j = 0; j < PAT_BITS - 1 && valid; ++j) {
                    if ((cur_pat >> j) & 1) {
                        if ((cur_pat >> (j + 1)) & 1) {              /* adjacent 1s */
                            /* if either column has a 1 above, current 1s would have
                               a second neighbour (vertical + horizontal) – disallow */
                            if ( (prev_pat >> j) & 1 ) { valid = false; break; }
                            if ( (prev_pat >> (j + 1)) & 1 ) { valid = false; break; }
                        }
                    }
                }
                if (!valid) continue;

                /* compute used mask for this row */
                uint16_t cur_used = 0;
                for (int j = 0; j < PAT_BITS; ++j) {
                    if ((cur_pat >> j) & 1) {
                        bool left  = (j > 0) && ((cur_pat >> (j - 1)) & 1);
                        bool right = (j < PAT_BITS-1) && ((cur_pat >> (j + 1)) & 1);
                        bool vert  = (prev_pat >> j) & 1;
                        if (left || right || vert) cur_used |= (1 << j);
                    }
                }

                int tstate = (cur_pat << PAT_BITS) | cur_used;
                _next_state[sidx][cnt++] = (uint16_t)tstate;
            }
            _trans_count[sidx] = cnt;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    _precompute();

    /* Dynamic programming arrays */
    static seq_t dp[2][STATE_COUNT];
    int cur = 0, nxt = 1;

    /* zero dp arrays */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp[cur][i] = dp[nxt][i] = 0;

    /* first row */
    for (int pat = 0; pat < PAT_COUNT; ++pat) {
        if (!_allowed[pat]) continue;
        uint16_t used = _horiz_mask[pat];
        dp[cur][(pat << PAT_BITS) | used] += (seq_t)1;
    }

    if (n == 1) {
        seq_t total = 0;
        for (size_t i = 0; i < STATE_COUNT; ++i) total += dp[cur][i];
        return total;
    }

    /* iterate subsequent rows */
    for (idx_t row = 2; row <= n; ++row) {
        /* zero next array */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp[nxt][i] = 0;

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp[cur][s];
            if (!val) continue;
            int cnt = _trans_count[s];
            const uint16_t *dest = _next_state[s];
            for (int k = 0; k < cnt; ++k)
                dp[nxt][dest[k]] += val;
        }

        /* swap current and next */
        int tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* sum over all final states */
    seq_t total = 0;
    for (size_t i = 0; i < STATE_COUNT; ++i) total += dp[cur][i];
    return total;
}
