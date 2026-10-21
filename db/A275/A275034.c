/*
 * A275034  Number of nX5 0..2 arrays with no element equal to any value at
 *          offset (0,-1) (-1,-2) or (-2,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×5 arrays with entries 0..2, no two
 * horizontally adjacent entries equal, no entry equal to the entry
 * at offsets (0,-1), (-1,-2) or (-2,0), and values introduced in order
 * 0,1,2 (restricted growth).  The implementation uses dynamic
 * programming over rows with pre‑computed transition tables.
 */

#include <stdbool.h>

/* constants */
#define COLS   5
#define VALS   3            /* 0,1,2 */
#define PATTERNS 48         /* 3*2*2*2*2 = 48 possible rows with no equal neighbours */

/* pattern values: patternVals[p][c] = value at column c in pattern p (0..2) */
static unsigned char patternVals[PATTERNS][COLS];

/* newMaxFrom[startIdx][p] = new maximum after placing pattern p
 * starting from maximum startMax, where startIdx = startMax+1
 * startMax values: -1 -> 0, 0 -> 1, 1 -> 2, 2 -> 3
 * If placement is impossible, value is -1. */
static signed char newMaxFrom[4][PATTERNS];

/* transition list for the second row (prev only, no prevPrev)
 * nextNone[p][k] = list of next row indices that satisfy (-1,-2) constraint with row p
 * nextNoneCnt[p] = number of such indices */
static unsigned char nextNone[PATTERNS][PATTERNS];
static unsigned char nextNoneCnt[PATTERNS];

/* transition list for rows >=3 (prevPrev and prev)
 * nextMap[prevPrev][prev][k] = list of next row indices that satisfy both constraints
 * nextMapCnt[prevPrev][prev] = number of such indices */
static unsigned char nextMap[PATTERNS][PATTERNS][PATTERNS];
static unsigned char nextMapCnt[PATTERNS][PATTERNS];

/* one‑time initializer */
static void init_static(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* ---- generate all admissible row patterns (no equal horizontal neighbours) ---- */
    int id = 0;
    for (int v0 = 0; v0 < VALS; ++v0)
    for (int v1 = 0; v1 < VALS; ++v1) if (v1 != v0)
    for (int v2 = 0; v2 < VALS; ++v2) if (v2 != v1)
    for (int v3 = 0; v3 < VALS; ++v3) if (v3 != v2)
    for (int v4 = 0; v4 < VALS; ++v4) if (v4 != v3)
    {
        patternVals[id][0] = (unsigned char)v0;
        patternVals[id][1] = (unsigned char)v1;
        patternVals[id][2] = (unsigned char)v2;
        patternVals[id][3] = (unsigned char)v3;
        patternVals[id][4] = (unsigned char)v4;
        ++id;
    }

    /* ---- precompute newMaxFrom table ---- */
    for (int startIdx = 0; startIdx < 4; ++startIdx)   /* startIdx 0..3 => startMax -1..2 */
    {
        int startMax = startIdx - 1;   /* conversion 0->-1, 1->0, etc. */
        for (int p = 0; p < PATTERNS; ++p)
        {
            int curMax = startMax;
            bool ok = true;
            for (int c = 0; c < COLS; ++c)
            {
                int v = patternVals[p][c];
                if (v > curMax + 1) { ok = false; break; }
                if (v > curMax) curMax = v;
            }
            newMaxFrom[startIdx][p] = ok ? curMax : -1;
        }
    }

    /* ---- precompute transition lists ---- */
    /* nextNone : only (-1,-2) constraint (no prevPrev) */
    for (int prev = 0; prev < PATTERNS; ++prev)
    {
        unsigned char *list = nextNone[prev];
        unsigned char cnt = 0;
        for (int nrow = 0; nrow < PATTERNS; ++nrow)
        {
            bool ok = true;
            for (int c = 2; c < COLS; ++c)
                if (patternVals[nrow][c] == patternVals[prev][c-2]) { ok = false; break; }
            if (ok) list[cnt++] = (unsigned char)nrow;
        }
        nextNoneCnt[prev] = cnt;
    }

    /* nextMap : both (-2,0) and (-1,-2) constraints */
    for (int prevPrev = 0; prevPrev < PATTERNS; ++prevPrev)
    {
        for (int prev = 0; prev < PATTERNS; ++prev)
        {
            unsigned char *list = nextMap[prevPrev][prev];
            unsigned char cnt = 0;
            for (int nrow = 0; nrow < PATTERNS; ++nrow)
            {
                bool ok = true;
                for (int c = 0; c < COLS; ++c)
                {
                    if (patternVals[nrow][c] == patternVals[prevPrev][c]) { ok = false; break; }
                    if (c >= 2 && patternVals[nrow][c] == patternVals[prev][c-2]) { ok = false; break; }
                }
                if (ok) list[cnt++] = (unsigned char)nrow;
            }
            nextMapCnt[prevPrev][prev] = cnt;
        }
    }
}

/* Main function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined below offset 1 */
    init_static();

    /* ----- DP for the first row ----- */
    seq_t dp1[PATTERNS][VALS] = {{0}};
    for (int p = 0; p < PATTERNS; ++p)
    {
        signed char m = newMaxFrom[0][p];   /* startIdx 0 => startMax = -1 */
        if (m >= 0) dp1[p][m] = 1;
    }

    /* answer for n==1 */
    if (n == 1)
    {
        seq_t ans = 0;
        for (int p = 0; p < PATTERNS; ++p)
            for (int m = 0; m < VALS; ++m)
                ans += dp1[p][m];
        return ans;
    }

    /* ----- DP for the first two rows ----- */
    static seq_t dp2[PATTERNS][PATTERNS][VALS];
    for (int a = 0; a < PATTERNS; ++a)
        for (int b = 0; b < PATTERNS; ++b)
            for (int m = 0; m < VALS; ++m)
                dp2[a][b][m] = 0;

    for (int first = 0; first < PATTERNS; ++first)
    {
        for (int m1 = 0; m1 < VALS; ++m1)
        {
            seq_t cnt = dp1[first][m1];
            if (cnt == 0) continue;
            unsigned char *list = nextNone[first];
            unsigned char cntList = nextNoneCnt[first];
            for (unsigned char idx = 0; idx < cntList; ++idx)
            {
                int second = list[idx];
                int newMax = newMaxFrom[m1 + 1][second];
                if (newMax >= 0)
                    dp2[first][second][newMax] += cnt;
            }
        }
    }

    /* answer for n==2 */
    if (n == 2)
    {
        seq_t ans = 0;
        for (int a = 0; a < PATTERNS; ++a)
            for (int b = 0; b < PATTERNS; ++b)
                for (int m = 0; m < VALS; ++m)
                    ans += dp2[a][b][m];
        return ans;
    }

    /* ----- DP for rows 3..n ----- */
    static seq_t dpA[PATTERNS][PATTERNS][VALS];
    static seq_t dpB[PATTERNS][PATTERNS][VALS];
    seq_t (*dpCurr)[PATTERNS][VALS] = dpA;
    seq_t (*dpNext)[PATTERNS][VALS] = dpB;

    /* copy dp2 into dpCurr */
    for (int a = 0; a < PATTERNS; ++a)
        for (int b = 0; b < PATTERNS; ++b)
            for (int m = 0; m < VALS; ++m)
                dpCurr[a][b][m] = dp2[a][b][m];

    for (idx_t row = 3; row <= n; ++row)
    {
        /* zero dpNext */
        for (int a = 0; a < PATTERNS; ++a)
            for (int b = 0; b < PATTERNS; ++b)
                for (int m = 0; m < VALS; ++m)
                    dpNext[a][b][m] = 0;

        for (int prevPrev = 0; prevPrev < PATTERNS; ++prevPrev)
        {
            for (int prev = 0; prev < PATTERNS; ++prev)
            {
                for (int m = 0; m < VALS; ++m)
                {
                    seq_t cnt = dpCurr[prevPrev][prev][m];
                    if (cnt == 0) continue;
                    unsigned char *list = nextMap[prevPrev][prev];
                    unsigned char cntList = nextMapCnt[prevPrev][prev];
                    for (unsigned char idx = 0; idx < cntList; ++idx)
                    {
                        int nrow = list[idx];
                        int newMax = newMaxFrom[m + 1][nrow];
                        if (newMax >= 0)
                            dpNext[prev][nrow][newMax] += cnt;
                    }
                }
            }
        }

        /* swap dpCurr and dpNext */
        seq_t (*tmp)[PATTERNS][VALS] = dpCurr;
        dpCurr = dpNext;
        dpNext = tmp;
    }

    /* sum final DP layer */
    seq_t ans = 0;
    for (int a = 0; a < PATTERNS; ++a)
        for (int b = 0; b < PATTERNS; ++b)
            for (int m = 0; m < VALS; ++m)
                ans += dpCurr[a][b][m];
    return ans;
}
