/*
 * A278091  Number of nX5 0..1 arrays with every element both equal and not
 *          equal to some elements at offset (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of n×5 0/1 arrays with upper left 0 and each
 *            element having both an equal and a not‑equal neighbor among
 *            offsets (-1,0) (-1,1) (0,-1) (0,1) (1,0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------- */
/*  Precomputation tables                                            */
/* --------------------------------------------------------------- */

#include <stdbool.h>   /* for bool, but not used in core logic */

#define STATE_COUNT   32768   /* 2^15  (5 bits pattern + 5 bits eq + 5 bits neq) */
#define PATTERN_COUNT 32      /* 5 bits pattern */

/* eqMask[prev][next] and neqMask[prev][next] hold the flags for
   the 'next' row after considering left/right and up/up‑right
   neighbours with previous row 'prev'.  */
static unsigned char eqMaskTable[PATTERN_COUNT][PATTERN_COUNT];
static unsigned char neqMaskTable[PATTERN_COUNT][PATTERN_COUNT];

/* nextState[s][p] gives the resulting state index after moving
   from state 's' with previous row pattern 'prev' to new row
   pattern 'p'.  -1 means the transition is invalid.  */
static short nextState[STATE_COUNT][PATTERN_COUNT];

/* ----------------------------------------------------------------- */
/* Helper for bit extraction                                        */
/* ----------------------------------------------------------------- */
static inline unsigned char bit_at(unsigned char pattern, int j)
{
    return (pattern >> j) & 1u;
}

/* ----------------------------------------------------------------- */
/* Build the transition tables once                                  */
/* ----------------------------------------------------------------- */
static bool tables_initialized = false;

static void initialize_tables(void)
{
    if (tables_initialized)
        return;
    tables_initialized = true;

    /* Precompute eqMaskTable and neqMaskTable */
    for (int prev = 0; prev < PATTERN_COUNT; ++prev)
    {
        for (int next = 0; next < PATTERN_COUNT; ++next)
        {
            unsigned char eqMask = 0;
            unsigned char neqMask = 0;

            for (int j = 0; j < 5; ++j)
            {
                unsigned char self   = bit_at((unsigned char)next, j);
                unsigned char left   = (j > 0)        ? bit_at((unsigned char)next, j - 1) : 0;
                unsigned char right  = (j < 4)        ? bit_at((unsigned char)next, j + 1) : 0;
                unsigned char up     = bit_at((unsigned char)prev, j);
                unsigned char upright= (j < 4)        ? bit_at((unsigned char)prev, j + 1) : 0;

                bool eq = false, neq = false;
                if (j > 0)
                {
                    if (left == self) eq = true;
                    else neq = true;
                }
                if (j < 4)
                {
                    if (right == self) eq = true;
                    else neq = true;
                }
                /* up is always present */
                if (up == self) eq = true;
                else neq = true;
                if (j < 4)
                {
                    if (upright == self) eq = true;
                    else neq = true;
                }
                if (eq)   eqMask |= (1u << j);
                if (neq) neqMask |= (1u << j);
            }
            eqMaskTable[prev][next]  = eqMask;
            neqMaskTable[prev][next] = neqMask;
        }
    }

    /* Build the state transition table */
    for (int state = 0; state < STATE_COUNT; ++state)
    {
        unsigned char prevPattern = state >> 10;
        unsigned char eqPrevMask   = (state >> 5) & 0x1Fu;
        unsigned char neqPrevMask  = state & 0x1Fu;

        for (int next = 0; next < PATTERN_COUNT; ++next)
        {
            bool valid = true;
            for (int j = 0; j < 5 && valid; ++j)
            {
                bool eqSat = ((eqPrevMask >> j) & 1u) ||
                            (bit_at(prevPattern, j) == bit_at((unsigned char)next, j));
                bool neqSat = ((neqPrevMask >> j) & 1u) ||
                              (bit_at(prevPattern, j) != bit_at((unsigned char)next, j));
                if (!eqSat || !neqSat)
                    valid = false;
            }
            if (!valid)
            {
                nextState[state][next] = -1;
                continue;
            }

            int newState = ((next << 10) |
                            (eqMaskTable[prevPattern][next] << 5) |
                            neqMaskTable[prevPattern][next]);
            nextState[state][next] = (short)newState;
        }
    }
}

/* ----------------------------------------------------------------- */
/* DP storage (two buffers and two lists of active states)          */
/* ----------------------------------------------------------------- */
static seq_t dp[2][STATE_COUNT];
static int activeIdx[2][STATE_COUNT];
static int activeSize[2];

/* ----------------------------------------------------------------- */
/* Utility: compute flags for the first row (only left/right        */
/* neighbours exist for each cell).                                 */
/* ----------------------------------------------------------------- */
static void compute_row0_flags(unsigned char pattern,
                               unsigned char *eqMask,
                               unsigned char *neqMask)
{
    unsigned char eMask = 0, nMask = 0;
    for (int j = 0; j < 5; ++j)
    {
        unsigned char self = bit_at(pattern, j);
        unsigned char left  = (j > 0)  ? bit_at(pattern, j - 1) : 0;
        unsigned char right = (j < 4)  ? bit_at(pattern, j + 1) : 0;
        bool eq = false, neq = false;
        if (j > 0)
        {
            if (left == self) eq = true;
            else neq = true;
        }
        if (j < 4)
        {
            if (right == self) eq = true;
            else neq = true;
        }
        if (eq)   eMask |= (1u << j);
        if (neq)  nMask |= (1u << j);
    }
    *eqMask = eMask;
    *neqMask = nMask;
}

/* ----------------------------------------------------------------- */
/* Main function                                                     */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)   /* offset is 1, but return 0 for lower indices */
        return (seq_t)0;

    initialize_tables();

    /* Reset DP buffers and active lists */
    for (int i = 0; i < STATE_COUNT; ++i)
    {
        dp[0][i] = 0;
        dp[1][i] = 0;
    }
    activeSize[0] = 0;
    activeSize[1] = 0;

    /* --- Initialize first row ------------------------------------ */
    int cur = 0;             /* current buffer index */
    int next = 1;            /* buffer for next iteration */

    for (int pattern = 0; pattern < PATTERN_COUNT; ++pattern)
    {
        /* upper left element must be 0 */
        if (pattern & 1) continue;

        unsigned char eMask, nMask;
        compute_row0_flags((unsigned char)pattern, &eMask, &nMask);
        int state = (pattern << 10) | (eMask << 5) | nMask;
        if (dp[cur][state] == 0)
            activeIdx[cur][activeSize[cur]++] = state;
        dp[cur][state] += (seq_t)1;
    }

    /* --- Process remaining rows ---------------------------------- */
    for (idx_t row = 1; row < n; ++row)
    {
        activeSize[next] = 0;   /* clear next active list */

        for (int sIdx = 0; sIdx < activeSize[cur]; ++sIdx)
        {
            int state = activeIdx[cur][sIdx];
            seq_t cnt = dp[cur][state];
            dp[cur][state] = 0;          /* clear */

            for (int pat = 0; pat < PATTERN_COUNT; ++pat)
            {
                short ns = nextState[state][pat];
                if (ns == -1) continue;

                if (dp[next][ns] == 0)
                    activeIdx[next][activeSize[next]++] = ns;
                dp[next][ns] += cnt;
            }
        }

        /* swap buffers and active lists */
        int tmp = cur; cur = next; next = tmp;
    }

    /* --- Final row check ---------------------------------------- */
    seq_t result = 0;
    for (int sIdx = 0; sIdx < activeSize[cur]; ++sIdx)
    {
        int state = activeIdx[cur][sIdx];
        unsigned char eqMask = (state >> 5) & 0x1Fu;
        unsigned char neqMask = state & 0x1Fu;
        if (eqMask == 0x1Fu && neqMask == 0x1Fu)
            result += dp[cur][state];
    }

    return result;
}
