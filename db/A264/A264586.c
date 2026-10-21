/*
 * A264586  Number of (3+1)X(n+1) arrays of permutations of 0..n*4+3 with each
 *          element having directed index change 1,0 0,-1 1,2 or -1,1.
 * https://oeis.org/A264586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×(n+1) arrays that are permutations of 0..4n+3
 *            * (A264586). The algorithm uses a transfer‑matrix DP with a
 *            16‑bit state * that tracks which rows in the previous, current,
 *            and next two columns * have already been used as targets. For
 *            each column it enumerates all * possible assig...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* State is 4 columns × 4 rows = 16 bits.
     * bits 0‑3   : column c‑1 (prev)
     * bits 4‑7   : column c   (curr)
     * bits 8‑11  : column c+1 (next)
     * bits 12‑15 : column c+2 (next2)  */
    const size_t STATES = 1U << 16;          /* 65536 */
    static seq_t *dpCurr = NULL;
    static seq_t *dpNext = NULL;
    static uint32_t *actCurr = NULL;         /* indices of active states */
    static uint32_t *actNext = NULL;
    static size_t actCap = 0;

    if (actCap == 0)                        /* one‑time allocation   */
    {
        actCap = STATES;
        dpCurr = (seq_t *)malloc(actCap * sizeof(seq_t));
        dpNext = (seq_t *)malloc(actCap * sizeof(seq_t));
        actCurr = (uint32_t *)malloc(actCap * sizeof(uint32_t));
        actNext = (uint32_t *)malloc(actCap * sizeof(uint32_t));
        for (size_t i = 0; i < actCap; ++i)
            dpCurr[i] = 0;                  /* initialise to zero */
    }

    /* initialise DP for column 0   */
    for (size_t i = 0; i < actCap; ++i)
        dpCurr[i] = 0;
    size_t actCurCnt = 0;
    dpCurr[0] = 1;
    actCurr[actCurCnt++] = 0;

    /* ------------------------------------------------------------------ */
    /*  Process columns 0 … n  */
    /* ------------------------------------------------------------------ */
    for (idx_t col = 0; col <= n; ++col)
    {
        size_t actNextCnt = 0;

        for (size_t ai = 0; ai < actCurCnt; ++ai)
        {
            uint16_t state  = actCurr[ai];
            seq_t   count   = dpCurr[state];
            uint8_t m0 = (state >> 0)  & 0xF;   /* column c‑1  */
            uint8_t m1 = (state >> 4)  & 0xF;   /* column c    */
            uint8_t m2 = (state >> 8)  & 0xF;   /* column c+1  */
            uint8_t m3 = (state >> 12) & 0xF;   /* column c+2  */

            /* build the list of allowed moves for the four rows in
             * current column, respecting column boundaries.   */
            int optCnt[4];
            int optCol[4][4];   /* -1: prev, 0: curr, 1: next, 2: next2 */
            int optRow[4][4];

            for (int r = 0; r < 4; ++r)
            {
                int o = 0;

                /* (1,0) : row+1, same column */
                if (r < 3) {
                    optCol[r][o] = 0;
                    optRow[r][o] = r + 1;
                    ++o;
                }

                /* (0,-1) : same row, prev column            */
                if (col > 0) {
                    optCol[r][o] = -1;
                    optRow[r][o] = r;
                    ++o;
                }

                /* (1,2) : row+1, next2 column               */
                if (col + 2 <= n && r < 3) {
                    optCol[r][o] = 2;
                    optRow[r][o] = r + 1;
                    ++o;
                }

                /* (-1,1) : row-1, next column               */
                if (col + 1 <= n && r > 0) {
                    optCol[r][o] = 1;
                    optRow[r][o] = r - 1;
                    ++o;
                }

                optCnt[r] = o;
            }

            /* Enumerate all 4‑tuple assignments (at most 4⁴ = 256).    */
            for (int o0 = 0; o0 < optCnt[0]; ++o0)
            {
                int col0 = optCol[0][o0];
                int row0 = optRow[0][o0];
                uint8_t tm0 = m0, tm1 = m1, tm2 = m2, tm3 = m3;
                bool ok0 = true;
                if (col0 == -1) { if (tm0 & (1U << row0)) ok0 = false; else tm0 |= (1U << row0); }
                else if (col0 == 0) { if (tm1 & (1U << row0)) ok0 = false; else tm1 |= (1U << row0); }
                else if (col0 == 1) { if (tm2 & (1U << row0)) ok0 = false; else tm2 |= (1U << row0); }
                else if (col0 == 2) { if (tm3 & (1U << row0)) ok0 = false; else tm3 |= (1U << row0); }
                if (!ok0) continue;

                for (int o1 = 0; o1 < optCnt[1]; ++o1)
                {
                    int col1 = optCol[1][o1];
                    int row1 = optRow[1][o1];
                    uint8_t s0 = tm0, s1 = tm1, s2 = tm2, s3 = tm3;
                    bool ok1 = true;
                    if (col1 == -1) { if (s0 & (1U << row1)) ok1 = false; else s0 |= (1U << row1); }
                    else if (col1 == 0) { if (s1 & (1U << row1)) ok1 = false; else s1 |= (1U << row1); }
                    else if (col1 == 1) { if (s2 & (1U << row1)) ok1 = false; else s2 |= (1U << row1); }
                    else if (col1 == 2) { if (s3 & (1U << row1)) ok1 = false; else s3 |= (1U << row1); }
                    if (!ok1) continue;

                    for (int o2 = 0; o2 < optCnt[2]; ++o2)
                    {
                        int col2 = optCol[2][o2];
                        int row2 = optRow[2][o2];
                        uint8_t t0 = s0, t1 = s1, t2 = s2, t3 = s3;
                        bool ok2 = true;
                        if (col2 == -1) { if (t0 & (1U << row2)) ok2 = false; else t0 |= (1U << row2); }
                        else if (col2 == 0) { if (t1 & (1U << row2)) ok2 = false; else t1 |= (1U << row2); }
                        else if (col2 == 1) { if (t2 & (1U << row2)) ok2 = false; else t2 |= (1U << row2); }
                        else if (col2 == 2) { if (t3 & (1U << row2)) ok2 = false; else t3 |= (1U << row2); }
                        if (!ok2) continue;

                        for (int o3 = 0; o3 < optCnt[3]; ++o3)
                        {
                            int col3 = optCol[3][o3];
                            int row3 = optRow[3][o3];
                            uint8_t u0 = t0, u1 = t1, u2 = t2, u3 = t3;
                            bool ok3 = true;
                            if (col3 == -1) { if (u0 & (1U << row3)) ok3 = false; else u0 |= (1U << row3); }
                            else if (col3 == 0) { if (u1 & (1U << row3)) ok3 = false; else u1 |= (1U << row3); }
                            else if (col3 == 1) { if (u2 & (1U << row3)) ok3 = false; else u2 |= (1U << row3); }
                            else if (col3 == 2) { if (u3 & (1U << row3)) ok3 = false; else u3 |= (1U << row3); }
                            if (!ok3) continue;

                            /* all 4 sources assigned – produce new state for next column   */
                            uint8_t newm0 = u0;
                            uint8_t newm1 = u1;
                            uint8_t newm2 = u2;
                            uint8_t newm3 = u3;      /* will become zero after shift */

                            uint16_t newState = (newm1 & 0xF)
                                              | ((newm2 & 0xF) << 4)
                                              | ((newm3 & 0xF) << 8);   /* m3 shifted out */

                            seq_t *dpRef = &dpNext[newState];
                            if (*dpRef == 0) {          /* first time this state reached in this column */
                                actNext[actNextCnt++] = newState;
                                *dpRef = count;
                            } else {
                                *dpRef += count;
                            }
                        }
                    }
                }
            }
        }

        /* clear old current DP entries so that dpCurr becomes all zeros */
        for (size_t i = 0; i < actCurCnt; ++i)
            dpCurr[actCurr[i]] = 0;

        /* swap pointers for next iteration  */
        seq_t   *tmpDP   = dpCurr; dpCurr   = dpNext; dpNext   = tmpDP;
        uint32_t *tmpAC  = actCurr; actCurr  = actNext; actNext  = tmpAC;
        actCurCnt = actNextCnt;
    }

    /* after all columns, the answer is any state where the last column
     * (now represented by m0) is fully occupied and the other columns
     * are empty.  */
    seq_t ans = 0;
    for (size_t i = 0; i < actCurCnt; ++i)
    {
        uint16_t state = actCurr[i];
        uint8_t m0 = (state >> 0)  & 0xF;
        uint8_t m1 = (state >> 4)  & 0xF;
        uint8_t m2 = (state >> 8)  & 0xF;
        /* m3 is already zero after the final shift   */
        if (m0 == 0xF && m1 == 0 && m2 == 0)
            ans += dpCurr[state];
    }

    return ans;
}
