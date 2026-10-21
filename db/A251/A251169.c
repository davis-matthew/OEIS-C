/*
 * A251169  Number of (n+1)X(2+1) 0..3 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements less than the absolute difference
 *          of its antidiagonal elements.
 * https://oeis.org/A251169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251169
 *  This function counts the number of (n+1)×3 arrays over the alphabet {0,1,2,3}
 *  such that no 2×2 subblock has the property
 *      min(diagonal)  <  |anti‑diagonal| .
 *
 *  The counting is performed by a dynamic‑programming walk over all possible
 *  3‑row patterns (there are 4³ = 64 of them).  For every step we transition
 *  from the current row pattern to a next row pattern only if the 2×2
 *  condition holds for the two overlapping columns.  The transition table
 *  is precomputed once for all 64² pairs.  The number of arrays of height
 *  n+1 (i.e. with n transitions) is the sum of the DP state after n steps.
 *  This algorithm runs in O(64·n) time and uses O(64) memory.
 */

#include <stdint.h>

/* Pre‑computed allowed transitions between row patterns */
static int _nextCnt[64];
static int _nextIdx[64][64];
static int _init_done = 0;

static void _init(void)
{
    int r1, r2, c;
    for (r1 = 0; r1 < 64; ++r1) {
        _nextCnt[r1] = 0;
        for (r2 = 0; r2 < 64; ++r2) {
            int ok = 1;
            for (c = 0; c < 2; ++c) {
                int tl = (r1 >> (2 * c))       & 3;
                int tr = (r1 >> (2 * (c + 1))) & 3;
                int bl = (r2 >> (2 * c))       & 3;
                int br = (r2 >> (2 * (c + 1))) & 3;
                int minDiag = tl < br ? tl : br;
                int diff    = tr > bl ? tr - bl : bl - tr;
                if (minDiag < diff) { ok = 0; break; }
            }
            if (ok) {
                _nextIdx[r1][_nextCnt[r1]++] = r2;
            }
        }
    }
    _init_done = 1;
}

seq_t a(idx_t n)
{
    if (!_init_done) _init();

    static seq_t cur[64];
    static seq_t nxt[64];
    seq_t *pc = cur;
    seq_t *pn = nxt;
    int i, j;

    /* first row can be any of the 64 patterns */
    for (i = 0; i < 64; ++i) pc[i] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* reset nxt array */
        for (i = 0; i < 64; ++i) pn[i] = 0;
        /* propagate counts */
        for (i = 0; i < 64; ++i) {
            seq_t val = pc[i];
            if (val == 0) continue;
            for (j = 0; j < _nextCnt[i]; ++j) {
                int r2 = _nextIdx[i][j];
                pn[r2] += val;
            }
        }
        /* swap pointers */
        seq_t *tmp = pc;
        pc = pn;
        pn = tmp;
    }

    /* sum over all end rows */
    seq_t total = 0;
    for (i = 0; i < 64; ++i) total += pc[i];
    return total;
}
