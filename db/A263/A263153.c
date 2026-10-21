/*
 * A263153  Number of (n+3)X(1+3) 0..1 arrays with each row and column not
 *          divisible by 9, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A263153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over the rows of a (n+3)×4 binary array.
   The DP keeps track of the residues of the four column numbers modulo 9
   after each processed row.  A state is a 4‑digit base‑9 number
   representation of the four residues.  There are 9⁴ = 6561 states.
   For every state we precompute the 14 possible successor states
   corresponding to the allowed 4‑bit row patterns (all 4‑bit numbers
   except 0 and 9).  After L = n+3 rows the answer is the sum of the
   DP counts of the states whose four residues are all non‑zero.  The
   DP is performed in O(L·6561·14) operations, which is fast enough
   for L≤10 000 even with arbitrary–precision `seq_t`. */

#include <stddef.h>
#include <stdbool.h>

/* Constants for the size of the DP tables */
#define STATE_COUNT   6561
#define PATTERN_COUNT 14

/* All 4‑bit numbers except 0 and 9 (1001₂).  The j‑th entry in a row
   is the bit of column j (j=0 is the most significant bit). */
static const int pattern_bits[PATTERN_COUNT][4] = {
    {0,0,0,1},   /* 0001 */
    {0,0,1,0},   /* 0010 */
    {0,0,1,1},   /* 0011 */
    {0,1,0,0},   /* 0100 */
    {0,1,0,1},   /* 0101 */
    {0,1,1,0},   /* 0110 */
    {0,1,1,1},   /* 0111 */
    {1,0,0,0},   /* 1000 */
    {1,0,1,0},   /* 1010 */
    {1,0,1,1},   /* 1011 */
    {1,1,0,0},   /* 1100 */
    {1,1,0,1},   /* 1101 */
    {1,1,1,0},   /* 1110 */
    {1,1,1,1}    /* 1111 */
};

/* Pre–computed transition table: trans[s][k] = next state after using
   pattern k when the current state is s.  The table is built once. */
static unsigned short trans[STATE_COUNT][PATTERN_COUNT];
static bool trans_initialized = false;

/* Helper to zero a DP array of seq_t */
static void zero_dp(seq_t *dp)
{
    for (int i = 0; i < STATE_COUNT; ++i)
        dp[i] = 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence is defined only for n≥1 */
        return 0;

    /* Initialise the transition table once */
    if (!trans_initialized) {
        for (int s = 0; s < STATE_COUNT; ++s) {
            int r0 = s % 9;
            int r1 = (s / 9) % 9;
            int r2 = (s / 81) % 9;
            int r3 = (s / 729) % 9;
            for (int k = 0; k < PATTERN_COUNT; ++k) {
                int nb0 = (2 * r0 + pattern_bits[k][0]) % 9;
                int nb1 = (2 * r1 + pattern_bits[k][1]) % 9;
                int nb2 = (2 * r2 + pattern_bits[k][2]) % 9;
                int nb3 = (2 * r3 + pattern_bits[k][3]) % 9;
                trans[s][k] = (unsigned short)(nb0 + nb1 * 9 + nb2 * 81 + nb3 * 729);
            }
        }
        trans_initialized = true;
    }

    /* Allocate DP arrays (static to avoid re‑allocation on every call) */
    static seq_t *dp   = NULL;
    static seq_t *dp2  = NULL;
    static int dp_allocated = 0;
    if (!dp_allocated) {
        dp  = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
        dp2 = (seq_t*)malloc(STATE_COUNT * sizeof(seq_t));
        dp_allocated = 1;
    }

    /* Length of the array in rows: L = n + 3 */
    size_t L = (size_t)n + 3;

    /* Initialise DP */
    zero_dp(dp);
    dp[0] = 1;                     /* all column residues start at 0 */

    /* Process each row */
    for (size_t step = 0; step < L; ++step) {
        zero_dp(dp2);               /* next DP row */
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (dp[s] == 0)
                continue;
            seq_t cur = dp[s];
            for (int k = 0; k < PATTERN_COUNT; ++k) {
                int t = trans[s][k];
                dp2[t] += cur;
            }
        }
        /* move to next step */
        for (int i = 0; i < STATE_COUNT; ++i)
            dp[i] = dp2[i];
    }

    /* Sum counts of states whose four residues are all non‑zero */
    seq_t result = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        int r0 = s % 9;
        int r1 = (s / 9) % 9;
        int r2 = (s / 81) % 9;
        int r3 = (s / 729) % 9;
        if (r0 && r1 && r2 && r3)
            result += dp[s];
    }
    return result;
}
