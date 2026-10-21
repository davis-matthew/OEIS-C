/*
 * A283547  Number of 6 X n 0..1 arrays with no 1 equal to more than one of
 *          its horizontal, diagonal and antidiagonal neighbors.
 * https://oeis.org/A283547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence: Number of 6×n 0..1 arrays with no 1 equal to more than
 * one of its horizontal, diagonal and antidiagonal neighbors.
 * This implementation uses a transfer‑matrix dynamic programming
 * that walks column by column.  The state consists of the last
 * two columns (previous two 6‑bit masks); it suffices because
 * every neighbour of a cell lies in either the previous or the
 * next column.  The transition from state (p,q) to state (q,r)
 * is allowed iff the column q satisfies the neighbour constraint
 * with the previous column p and the new column r.  The DP
 * runs in O(n·S·T) where S=4096 is the number of states and
 * T is the average number of outgoing transitions (<20).  This
 * easily meets the 2 second requirement for n up to 10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define STATE_BITS 6
#define MASK_SIZE (1 << STATE_BITS)   /* 64 */
#define STATE_SIZE (MASK_SIZE * MASK_SIZE)  /* 4096 */

/* Function to test whether a column q (mask2) is valid given its
 * left column p (mask1) and right column r (mask3).  Returns
 * true if for every 1 bit in q the number of 1 neighbours in
 * (p,r) is at most one. */
static bool
valid_mask(uint8_t p, uint8_t q, uint8_t r)
{
    for (int row = 0; row < STATE_BITS; ++row) {
        if ((q >> row) & 1U) {          /* cell (row, current) is 1 */
            int cnt = 0;
            /* left (p) column */
            if ((p >> row) & 1U) ++cnt;
            if (row > 0 && ((p >> (row - 1)) & 1U)) ++cnt;
            if (row + 1 < STATE_BITS && ((p >> (row + 1)) & 1U)) ++cnt;
            /* right (r) column */
            if ((r >> row) & 1U) ++cnt;
            if (row > 0 && ((r >> (row - 1)) & 1U)) ++cnt;
            if (row + 1 < STATE_BITS && ((r >> (row + 1)) & 1U)) ++cnt;
            if (cnt > 1) return false;
        }
    }
    return true;
}

/* Memoised transition lists ------------------------------------------------- */
static bool     valid_tbl[STATE_SIZE][256]; /* pivot: valid_tbl[p][q*256 + r] */
static uint8_t  next_list[STATE_SIZE][MASK_SIZE]; /* up to 64 per state */
static uint8_t  next_cnt[STATE_SIZE];            /* number of entries in next_list */

/* Initialise tables on first call ------------------------------------------- */
static void init_tables(void)
{
    static int init_done = 0;
    if (init_done) return;
    init_done = 1;

    for (int p = 0; p < MASK_SIZE; ++p) {
        for (int q = 0; q < MASK_SIZE; ++q) {
            int t = p * MASK_SIZE + q;   /* state id */
            uint8_t c = 0;
            for (int r = 0; r < MASK_SIZE; ++r) {
                bool ok = valid_mask((uint8_t)p, (uint8_t)q, (uint8_t)r);
                if (ok) {
                    next_list[t][c++] = (uint8_t)r;
                }
            }
            next_cnt[t] = c;
            /* Cache validity of transition (p,q,r) for quick final check */
            for (int r = 0; r < MASK_SIZE; ++r) {
                valid_tbl[t][r] = valid_mask((uint8_t)p, (uint8_t)q, (uint8_t)r);
            }
        }
    }
}

/* The requested sequence function ------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n=0 in the OEIS */
    init_tables();

    /* DP arrays: previous and current layer */
    static seq_t dp_prev[STATE_SIZE];
    static seq_t dp_curr[STATE_SIZE];

    /* Initialize: position 0, previous column = 0 */
    for (int i = 0; i < STATE_SIZE; ++i) dp_prev[i] = 0;
    for (int m = 0; m < MASK_SIZE; ++m) {
        int idx = 0 * MASK_SIZE + m;   /* previous = 0, current = m */
        dp_prev[idx] = 1;
    }

    /* Process columns 1 .. n-1 */
    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < STATE_SIZE; ++i) dp_curr[i] = 0;
        for (int s = 0; s < STATE_SIZE; ++s) {
            seq_t ways = dp_prev[s];
            if (ways == 0) continue;
            uint8_t count = next_cnt[s];
            const uint8_t *list = next_list[s];
            uint8_t prev = s & 0x3F;          /* current column mask */
            for (uint8_t k = 0; k < count; ++k) {
                uint8_t r = list[k];
                int new_state = (prev << STATE_BITS) | r; /* encode (prev, r) */
                dp_curr[new_state] += ways;
            }
        }
        /* swap layers */
        for (int i = 0; i < STATE_SIZE; ++i) dp_prev[i] = dp_curr[i];
    }

    /* Final summation: last column has no right neighbour */
    seq_t total = 0;
    for (int s = 0; s < STATE_SIZE; ++s) {
        int prevprev = s >> STATE_BITS;
        int prev     = s & 0x3F;
        if (valid_tbl[prevprev * MASK_SIZE + prev][0]) { /* r = 0 */
            total += dp_prev[s];
        }
    }
    return total;
}
