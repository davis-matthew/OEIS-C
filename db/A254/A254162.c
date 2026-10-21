/*
 * A254162  Number of (n+2)X(2+2) 0..1 arrays with every 3X3 subblock diagonal
 *          maximum minus antidiagonal median nondecreasing horizontally,
 *          vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A254162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A254162 – Number of (n+2)×4 binary arrays with the
 *  diagonal‑max minus anti‑diagonal‑median value of each 3×3
 *  subblock nondecreasing horizontally, vertically and
 *  NE→SW.  The implementation builds a DFA over 3‑row windows,
 *  precomputes valid transitions, and performs DP over the
 *  height of the array.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Number of possible 3×4 binary states (4 rows×4 columns) */
enum { STATE_CNT = 4096 };

/* Transition data structures */
static signed char leftDiff[STATE_CNT];
static signed char rightDiff[STATE_CNT];
static uint8_t    nextCnt[STATE_CNT];
static int       nextState[STATE_CNT][16];
static bool init_done = false;

/* ------------------------------------------------------------------
 * Initialise the transition table once
 * ------------------------------------------------------------------ */
static void init_states(void)
{
    if (init_done) return;
    init_done = true;

    for (int s = 0; s < STATE_CNT; ++s) {
        /* Split the 12 bits of s into three rows of 4 bits. */
        int r0 = (s >> 8) & 0xF;   /* top row */
        int r1 = (s >> 4) & 0xF;   /* middle row */
        int r2 =  s        & 0xF;   /* bottom row */

        /* Left 3×3 block (columns 0‑2). */
        int dmax  = ((r0 >> 0) & 1) | ((r1 >> 1) & 1) | ((r2 >> 2) & 1);
        int a1    = (r0 >> 2) & 1;   /* upper‑right corner of block */
        int a2    = (r1 >> 1) & 1;   /* centre of anti‑diagonal */
        int a3    = (r2 >> 0) & 1;   /* lower‑left corner of block */
        int median = ((a1 + a2 + a3) >= 2) ? 1 : 0;
        leftDiff[s] = (signed char)(dmax - median);

        /* Right 3×3 block (columns 1‑3). */
        dmax  = ((r0 >> 1) & 1) | ((r1 >> 2) & 1) | ((r2 >> 3) & 1);
        a1    = (r0 >> 3) & 1;   /* upper‑left corner of block */
        a2    = (r1 >> 2) & 1;   /* centre of anti‑diagonal */
        a3    = (r2 >> 1) & 1;   /* lower‑right corner of block */
        median = ((a1 + a2 + a3) >= 2) ? 1 : 0;
        rightDiff[s] = (signed char)(dmax - median);
    }

    /* Build transition table */
    for (int s = 0; s < STATE_CNT; ++s) {
        /* Horizontal monotonicity of the initial state's two subblocks */
        if (leftDiff[s] > rightDiff[s]) continue;

        /* The middle two rows become the top two rows of the next state. */
        int b = (s >> 4) & 0xF;   /* row 1 of current state */
        int c =  s        & 0xF;   /* row 2 of current state */

        for (int r3 = 0; r3 < 16; ++r3) {          /* new bottom row */
            int ns = (b << 8) | (c << 4) | r3;     /* next state */

            /* Horizontal monotonicity of the new state's subblocks */
            if (leftDiff[ns] > rightDiff[ns]) continue;

            /* Vertical monotonicity */
            if (leftDiff[s]   > leftDiff[ns])   continue;
            if (rightDiff[s]  > rightDiff[ns])  continue;

            /* NE→SW monotonicity (two constraints) */
            if (leftDiff[s]   > rightDiff[ns])   continue;  /* left→right */
            if (rightDiff[s]  > leftDiff[ns])    continue;  /* right→left */

            int idx = nextCnt[s]++;
            nextState[s][idx] = ns;
        }
    }
}

/* ------------------------------------------------------------------
 * The main function requested by the OEIS wrapper
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_states();

    /* Height of the array is n+2 rows. */
    const idx_t H = n + 2;

    /* DP arrays: 4096 states.  Static so that their size is a
       compile‑time constant (enum gives the constant expression). */
    static seq_t dp[STATE_CNT];
    static seq_t newdp[STATE_CNT];

    /* Clear DP arrays for this call. */
    memset(dp,   0, sizeof(dp));
    memset(newdp,0, sizeof(newdp));

    /* Initial states: need left ≤ right for the very first 3×3
       subblock. */
    for (int s = 0; s < STATE_CNT; ++s) {
        if (leftDiff[s] <= rightDiff[s]) dp[s] = 1;
    }

    /* Number of transitions = H - 3 rows beyond the first window. */
    const size_t steps = (size_t)(H) > 3 ? (size_t)(H) - 3 : 0;

    for (size_t step = 0; step < steps; ++step) {
        memset(newdp, 0, sizeof(newdp));

        for (int s = 0; s < STATE_CNT; ++s) {
            if (!dp[s]) continue;
            seq_t val = dp[s];
            uint8_t cnt = nextCnt[s];
            for (int k = 0; k < cnt; ++k) {
                int ns = nextState[s][k];
                newdp[ns] += val;
            }
        }

        /* Prepare for next iteration */
        memcpy(dp, newdp, sizeof(dp));
    }

    /* Sum over all final states; they already satisfy horizontal
       constraints by construction. */
    seq_t result = 0;
    for (int s = 0; s < STATE_CNT; ++s) result += dp[s];
    return result;
}
