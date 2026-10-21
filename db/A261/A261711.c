/*
 * A261711  Triangle read by rows: T(n,k) is the number of words over alphabet
 *          {0,1,2,3} having exactly k occurrences of the string 01, where
 *          n>=0 and k>=0.
 * https://oeis.org/A261711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   G.f.: 1/(1-4*x-(y-1)*x^2). The code computes the triangle T(n,k)
 *            (number of words over {0,1,2,3} of length n with exactly k
 *            occurrences of the substring "01") row by row by dynamic
 *            programming and returns the n-th element in the triangle read by
 *            rows
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 80, u64 n <= 289,
 *            u128 n <= 1123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
#include <stdlib.h>
#include <stdint.h>

/* Maximum number of 01‑occurrences needed for rows up to the
   required input range.  The required size for n ≤ 10000 is far
   smaller (kmax ≤ 100), but 512 is ample for safety. */
#define MAX_K 512

/* local DP for one row -----------------------------------------------*/
static void compute_row(idx_t row, seq_t *out)
{
    int kmax = (int)(row >> 1);      /* floor(row/2) */

    /* DP arrays: dpPrev[p][c] -- number of words of current length
       ending with symbol p (0..3) and having c occurrences of "01". */
    seq_t dpPrev[4][MAX_K] = {0};
    seq_t dpCurr[4][MAX_K] = {0};

    if (row == 0) {
        out[0] = 1;   /* empty word */
        return;
    }

    /* initialise for length 1 */
    for (int p = 0; p < 4; ++p)
        dpPrev[p][0] = 1;

    /* extend the word one symbol at a time */
    for (int pos = 2; pos <= (int)row; ++pos) {
        int newKmax = pos >> 1;          /* maximum occurrences possible */
        int prevMax = (pos - 1) >> 1;    /* previous maximum */

        /* zero the new DP array up to newKmax */
        for (int p = 0; p < 4; ++p)
            for (int c = 0; c <= newKmax; ++c)
                dpCurr[p][c] = 0;

        for (int prev = 0; prev < 4; ++prev) {
            for (int cnt = 0; cnt <= prevMax; ++cnt) {
                seq_t val = dpPrev[prev][cnt];
                if (!val) continue;
                for (int nxt = 0; nxt < 4; ++nxt) {
                    int inc = (prev == 0 && nxt == 1) ? 1 : 0;
                    int newCnt = cnt + inc;
                    if (newCnt <= newKmax)
                        dpCurr[nxt][newCnt] += val;
                }
            }
        }

        /* prepare for next step: copy dpCurr to dpPrev */
        for (int p = 0; p < 4; ++p)
            for (int c = 0; c <= newKmax; ++c)
                dpPrev[p][c] = dpCurr[p][c];
    }

    /* collect the results for this row */
    for (int cnt = 0; cnt <= kmax; ++cnt) {
        seq_t total = 0;
        for (int p = 0; p < 4; ++p)
            total += dpPrev[p][cnt];
        out[cnt] = total;
    }
}

/* public function -----------------------------------------------------*/
seq_t a(idx_t n)
{
    idx_t cum = 0;   /* number of terms already accounted for */

    for (idx_t r = 0; ; ++r) {
        int kmax   = (int)(r >> 1);   /* floor(r/2)          */
        int rowLen = kmax + 1;        /* number of terms in row r */

        if (cum + rowLen > n) {       /* target row found   */
            int k = (int)(n - cum);   /* column index       */
            seq_t rowbuf[MAX_K];
            compute_row(r, rowbuf);
            return rowbuf[k];
        }

        cum += rowLen;   /* move to next row   */
    }
}
