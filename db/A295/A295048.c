/*
 * A295048  Number of nX5 0..1 arrays with each 1 horizontally or vertically
 *          adjacent to 0 or 2 1s.
 * https://oeis.org/A295048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 0..1 arrays in which every 1 has 0 or 2
 *            1‑neighbours (horizontal or vertical). The computation uses a
 *            transfer‑matrix DP over 1024 states (mask of vertical edge from
 *            the row above and the pattern of the current row). All
 *            arithmetic is performed in seq_t, the implementation ty...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Constants for DP */
#define WIDTH     5
#define STATE_BITS 10          /* 5 bits for mask, 5 bits for pattern */
#define NUM_STATES (1 << STATE_BITS) /* 1024 */

/* Transition tables and final validity array.  They are initialized
   exactly once on the first call to a(). */
static bool tables_initialized = false;
static unsigned char trans_len[NUM_STATES];
static unsigned short trans_to[NUM_STATES][32];
static bool final_valid[NUM_STATES];
static unsigned char hdeg[32][WIDTH];   /* horizontal degree for each pattern */

/* Table initialisation ---------------------------------------------------- */
static void init_tables(void)
{
    if (tables_initialized)
        return;

    /* Precompute horizontal degrees for every pattern */
    for (unsigned int p = 0; p < 32; ++p) {
        for (int j = 0; j < WIDTH; ++j) {
            if ((p >> j) & 1U) {            /* cell is 1 */
                int h = 0;
                if (j > 0 && (p >> (j - 1)) & 1U)  h++;
                if (j < WIDTH - 1 && (p >> (j + 1)) & 1U) h++;
                hdeg[p][j] = (unsigned char)h;
            } else
                hdeg[p][j] = 0;
        }
    }

    /* Build transitions and final validity for every state */
    for (unsigned int mask = 0; mask < 32; ++mask) {
        for (unsigned int p = 0; p < 32; ++p) {
            unsigned int state = (mask << WIDTH) | p;
            unsigned char cnt = 0;

            for (unsigned int q = 0; q < 32; ++q) {
                bool ok = true;
                /* Check validity when q is added after p */
                for (int j = 0; j < WIDTH; ++j) {
                    unsigned char p_bit = (p >> j) & 1U;
                    unsigned char q_bit = (q >> j) & 1U;
                    unsigned char v_above = (mask >> j) & 1U;     /* already from above */
                    unsigned char v_curr   = p_bit & q_bit;      /* vertical between p and q */

                    /* Final degree of a cell of previous row */
                    if (p_bit) {
                        unsigned char final = hdeg[p][j] + v_above + v_curr;
                        if (final != 0 && final != 2) { ok = false; break; }
                    }

                    /* Partial degree of a cell of current row */
                    if (q_bit) {
                        unsigned char partial = hdeg[q][j] + v_curr;
                        if (partial > 2) { ok = false; break; }
                    }
                }
                if (ok) {
                    unsigned int new_mask = p & q;
                    unsigned int new_state = (new_mask << WIDTH) | q;
                    trans_to[state][cnt++] = (unsigned short)new_state;
                }
            }
            trans_len[state] = cnt;

            /* Determine whether this state can be final */
            bool fin_ok = true;
            for (int j = 0; j < WIDTH; ++j) {
                unsigned char p_bit = (p >> j) & 1U;
                if (p_bit) {
                    unsigned char final = hdeg[p][j] + ((mask >> j) & 1U);
                    if (final != 0 && final != 2) { fin_ok = false; break; }
                }
            }
            final_valid[state] = fin_ok;
        }
    }

    tables_initialized = true;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();

    /* DP arrays */
    seq_t dp[NUM_STATES]   = {0};
    seq_t dp_next[NUM_STATES] = {0};

    /* Initialise first row: mask = 0, pattern = p */
    for (unsigned int p = 0; p < 32; ++p) {
        unsigned int key = p;           /* mask=0 */
        dp[key] = 1;
    }

    /* Special case n == 1 */
    if (n == 1) {
        seq_t res = 0;
        for (unsigned int s = 0; s < NUM_STATES; ++s)
            if (final_valid[s])
                res += dp[s];
        return res;
    }

    /* Iterate rows 2 .. n */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero dp_next */
        for (unsigned int i = 0; i < NUM_STATES; ++i)
            dp_next[i] = 0;

        /* Transitions */
        for (unsigned int s = 0; s < NUM_STATES; ++s) {
            if (dp[s] == 0)
                continue;
            unsigned char c = trans_len[s];
            seq_t val = dp[s];
            for (unsigned char t = 0; t < c; ++t) {
                unsigned int to = trans_to[s][t];
                dp_next[to] += val;
            }
        }

        /* Prepare for next iteration */
        for (unsigned int i = 0; i < NUM_STATES; ++i)
            dp[i] = dp_next[i];
    }

    /* Sum over final valid states to obtain a(n) */
    seq_t res = 0;
    for (unsigned int s = 0; s < NUM_STATES; ++s)
        if (final_valid[s])
            res += dp[s];

    return res;
}
