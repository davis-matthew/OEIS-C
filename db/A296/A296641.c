/*
 * A296641  Number of nX6 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 1, 2 or 4 neighboring 1s.
 * https://oeis.org/A296641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Transitive DP over 3-row windows for 6‑column binary arrays *
 *            with each 1 having 1, 2 or 4 neighbors among its 6 adjacencies *
 *            (horizontal, vertical or antidiagonal). The function uses a *
 *            precomputed transition table for pairs of rows, achieving O(n)
 *            time * with a state space of 64² = 4096
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define COLS 6
#define ROWS (1 << COLS)          /* 64 possible rows */
#define STATES (ROWS * ROWS)      /* 4096 states (prev2,row) */

/* Convert a pair of rows into a state index */
static inline size_t idx(uint8_t a, uint8_t b)
{
    return ((size_t)a << COLS) | b;
}

/* Precomputed transitions: for a given (prev2, prev) the allowed next
 * rows that keep the middle row consistent. */
static uint8_t transitions[STATES][ROWS];
static uint8_t trans_len[STATES];
static bool trans_init = false;

/* Check whether 'mid' row is valid given 'above' and 'below' rows.
 * Adjacency is defined as follows for cell (i,j) with 1:
 *  - horizontal: (i,j-1), (i,j+1)
 *  - vertical:   (i-1,j), (i+1,j)
 *  - antidiagonal: (i-1,j+1), (i+1,j-1)
 *  The number of neighboring 1s must be 1, 2, or 4.           */
static bool is_valid(uint8_t above, uint8_t mid, uint8_t below)
{
    for (int col = 0; col < COLS; ++col)
    {
        if ((mid >> col) & 1)
        {
            int cnt = 0;
            /* horizontal */
            if (col > 0          && ((mid >> (col - 1)) & 1)) cnt++;
            if (col < COLS - 1   && ((mid >> (col + 1)) & 1)) cnt++;
            /* vertical */
            if ((above >> col) & 1) cnt++;
            if ((below >> col) & 1) cnt++;
            /* antidiagonal */
            if (col < COLS - 1   && ((above >> (col + 1)) & 1)) cnt++;
            if (col > 0          && ((below >> (col - 1)) & 1)) cnt++;
            if (cnt != 1 && cnt != 2 && cnt != 4)
                return false;
        }
    }
    return true;
}

/* Build transition tables once */
static void init_transitions(void)
{
    if (trans_init)
        return;
    for (uint8_t above = 0; above < ROWS; ++above)
    {
        for (uint8_t mid = 0; mid < ROWS; ++mid)
        {
            size_t s = idx(above, mid);
            uint8_t *dst = transitions[s];
            uint8_t *len = &trans_len[s];
            *len = 0;
            for (uint8_t below = 0; below < ROWS; ++below)
            {
                if (is_valid(above, mid, below))
                    dst[(*len)++] = below;
            }
        }
    }
    trans_init = true;
}

/* Count of solutions for an n x 6 binary array satisfying the rule. */
seq_t a(idx_t n)
{
    init_transitions();

    /* dp arrays for the 4096 states */
    static seq_t *dp_cur = NULL, *dp_nxt = NULL;
    if (!dp_cur)
    {
        dp_cur = (seq_t*)malloc(sizeof(seq_t) * STATES);
        dp_nxt = (seq_t*)malloc(sizeof(seq_t) * STATES);
    }

    /* Zero dp arrays */
    for (size_t i = 0; i < STATES; ++i) dp_cur[i] = 0;
    /* Initial state : (0,0) */
    dp_cur[0] = (seq_t)1;

    for (idx_t row = 0; row < n; ++row)
    {
        /* Zero next array */
        for (size_t i = 0; i < STATES; ++i) dp_nxt[i] = 0;

        for (size_t state = 0; state < STATES; ++state)
        {
            seq_t val = dp_cur[state];
            if (val == 0)
                continue;
            uint8_t above = state >> COLS;     /* prev2 */
            uint8_t mid   = state & (ROWS - 1);/* prev */

            uint8_t *lst = transitions[state];
            uint8_t l    = trans_len[state];
            for (uint8_t k = 0; k < l; ++k)
            {
                uint8_t below = lst[k];
                size_t new_state = idx(mid, below);
                dp_nxt[new_state] += val;
            }
        }

        /* Swap dp arrays */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* Sum all states where the last row (mid) is valid with dummy 0 row */
    seq_t result = 0;
    for (size_t state = 0; state < STATES; ++state)
    {
        uint8_t above = state >> COLS;   /* row n-1 */
        uint8_t mid   = state & (ROWS - 1);/* row n */
        if (is_valid(above, mid, 0))
            result += dp_cur[state];
    }

    return result;
}
