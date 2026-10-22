/*
 * A302270  Number of 6 X n 0..1 arrays with every element equal to 0, 1, 2 or
 *          3 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302270  (Number of 6 × n arrays of 0..3 with the upper‑left
 *  element 0 and at most three equal neighbours for every entry.
 *
 *  The implementation uses dynamic programming over columns.
 *  It keeps the set of all 6‑bit column vectors (0..63).
 *  For each pair of consecutive columns (prev, cur) it
 *  pre‑computes the list of columns nxt that can follow them
 *  without violating the neighbour condition.
 *  The first transition is handled separately because the left
 *  neighbour of the first column does not exist.
 *
 *  Complexity: O(n * 64³) but in practice the number of valid
 *  transitions per state is far smaller, and the implementation
 *  runs in well under two seconds for n ≈ 10‑000.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -------------------------------------------------------------------- */
/*  Auxiliary tables                                              */
/* -------------------------------------------------------------------- */

/* List of valid next columns for pair (prev, cur); size[][] tracks
 * the number of entries in each list.  All vectors are 6‑bit numbers
 * 0..63.  startList and startLen handle the first transition (prev =
 * “outside” i.e. 0, and no left neighbour). */
static unsigned char startList[64][64];
static unsigned char startLen[64];
static unsigned char nextList[64][64][64];
static unsigned char nextLen[64][64];

/* -------------------------------------------------------------------- */
/*  Helper predicates                                           */
/* -------------------------------------------------------------------- */

/* Return true if the three columns (prev, cur, nxt) satisfy the
 * neighbour condition.  has_prev indicates whether a left neighbour
 * exists; has_next whether a right neighbour exists. */
static inline bool valid_pair(unsigned char prev, unsigned char cur,
                               unsigned char nxt,
                               bool has_prev, bool has_next)
{
    for (int r = 0; r < 6; ++r) {
        unsigned int val  = (cur >> r) & 1u;
        unsigned int same = 0;

        /* left neighbour */
        if (has_prev && ((prev >> r) & 1u) == val) ++same;
        /* right neighbour */
        if (has_next && ((nxt >> r) & 1u) == val) ++same;
        /* left‑up */
        if (has_prev && r > 0 && ((prev >> (r-1)) & 1u) == val) ++same;
        /* left‑down */
        if (has_prev && r < 5 && ((prev >> (r+1)) & 1u) == val) ++same;
        /* right‑up */
        if (has_next && r > 0 && ((nxt >> (r-1)) & 1u) == val) ++same;
        /* right‑down */
        if (has_next && r < 5 && ((nxt >> (r+1)) & 1u) == val) ++same;

        if (same > 3u) return false;
    }
    return true;
}

/* Validate the last column: only a left neighbour exists. */
static inline bool valid_last(unsigned char prev, unsigned char cur)
{
    return valid_pair(prev, cur, 0, true, false);
}

/* -------------------------------------------------------------------- */
/*  Static precomputation of transition tables                      */
/* -------------------------------------------------------------------- */
static void init_lists(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* startList : first transition (no left neighbour). */
    for (unsigned char cur = 0; cur < 64; ++cur) {
        startLen[cur] = 0;
        for (unsigned char nxt = 0; nxt < 64; ++nxt) {
            if (valid_pair(0, cur, nxt, false, true)) {
                startList[cur][startLen[cur]++] = nxt;
            }
        }
    }

    /* nextList : general transition with both neighbours present. */
    for (unsigned char prev = 0; prev < 64; ++prev) {
        for (unsigned char cur = 0; cur < 64; ++cur) {
            nextLen[prev][cur] = 0;
            for (unsigned char nxt = 0; nxt < 64; ++nxt) {
                if (valid_pair(prev, cur, nxt, true, true)) {
                    nextList[prev][cur][nextLen[prev][cur]++] = nxt;
                }
            }
        }
    }
}

/* -------------------------------------------------------------------- */
/*  Main sequence function                                      */
/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    init_lists();                   /* ensure transition tables ready */

    /* The definition starts at n = 1.  Any other behaviour is
     * unspecified; we return 0 for n == 0 for safety. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)32;   /* 2^(6-1) = 32 matrices of width 1 */

    /* Dynamic‑programming tables.  We only need two consecutive
     * columns of states; each state is a pair (prev, cur) of
     * 6‑bit vectors (64 possibilities each). */
    seq_t dp0[64][64];
    seq_t dp1[64][64];
    seq_t (*dp_cur)[64] = dp0;      /* pointer to current DP table */
    seq_t (*dp_nxt)[64] = dp1;      /* pointer to next DP table */

    /* Initialise current table to zero. */
    for (unsigned int i = 0; i < 64; ++i)
        for (unsigned int j = 0; j < 64; ++j)
            dp_cur[i][j] = (seq_t)0;

    /* First column: the left neighbour does not exist, so we
     * fix the top element to 0. */
    for (unsigned char prev = 0; prev < 64; ++prev) {
        if (prev & 1u) continue;              /* top element must be 0 */
        unsigned char *list = startList[prev];
        unsigned char len   = startLen[prev];
        for (unsigned char k = 0; k < len; ++k) {
            unsigned char nxt = list[k];
            dp_cur[prev][nxt] = (seq_t)1;      /* one way to reach it */
        }
    }

    /* Process columns 3 … n .  After the initial step we have
     * already processed up to column 2, so we start from column 3. */
    for (idx_t col = 3; col <= n; ++col) {
        /* Zero the next table. */
        for (unsigned int i = 0; i < 64; ++i)
            for (unsigned int j = 0; j < 64; ++j)
                dp_nxt[i][j] = (seq_t)0;

        /* Transition from every (prev, cur) state to each allowed nxt. */
        for (unsigned char prev = 0; prev < 64; ++prev) {
            for (unsigned char cur = 0; cur < 64; ++cur) {
                seq_t val = dp_cur[prev][cur];
                if (val == (seq_t)0) continue;
                unsigned char *list = nextList[prev][cur];
                unsigned char len   = nextLen[prev][cur];
                for (unsigned char k = 0; k < len; ++k) {
                    unsigned char nxt = list[k];
                    dp_nxt[cur][nxt] += val;
                }
            }
        }

        /* Swap current and next tables for the next iteration. */
        seq_t (*tmp)[64] = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* Final column: sum over all states that satisfy the right‑border
     * condition (no right neighbour). */
    seq_t result = (seq_t)0;
    for (unsigned char prev = 0; prev < 64; ++prev) {
        for (unsigned char cur = 0; cur < 64; ++cur) {
            seq_t val = dp_cur[prev][cur];
            if (val == (seq_t)0) continue;
            if (valid_last(prev, cur))
                result += val;
        }
    }

    return result;
}
