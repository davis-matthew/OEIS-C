/*
 * A298627  Number of nX6 0..1 arrays with every element equal to 2, 3, 4, 5
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×6 binary arrays with upper‑left zero and
 *            every cell having 2,3,4,5 or 7 king‑move adjacent cells equal to
 *            itself
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 201 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check that for each cell in middle row 'mid', the number of king‑move
   adjacent cells equal to midbit is in {2,3,4,5,7}.  The arguments
   'has_prev' and 'has_next' indicate whether the row above or below
   exists; if not, those neighbors are ignored.                       */
static inline bool row_ok_generic(bool has_prev, uint8_t prev,
                                  bool has_next, uint8_t next,
                                  uint8_t mid)
{
    for (int col = 0; col < 6; ++col) {
        uint8_t midbit = (mid >> col) & 1;
        int cnt = 0;

        /* left/right neighbors in the same row */
        if (col > 0)  cnt += ((mid >> (col - 1)) & 1) == midbit;
        if (col < 5)  cnt += ((mid >> (col + 1)) & 1) == midbit;

        /* neighbors from the row above */
        if (has_prev) {
            if (col > 0)  cnt += ((prev >> (col - 1)) & 1) == midbit;
            cnt += ((prev >> col) & 1) == midbit;
            if (col < 5)  cnt += ((prev >> (col + 1)) & 1) == midbit;
        }

        /* neighbors from the row below */
        if (has_next) {
            if (col > 0)  cnt += ((next >> (col - 1)) & 1) == midbit;
            cnt += ((next >> col) & 1) == midbit;
            if (col < 5)  cnt += ((next >> (col + 1)) & 1) == midbit;
        }

        if (!(cnt == 2 || cnt == 3 || cnt == 4 || cnt == 5 || cnt == 7))
            return false;
    }
    return true;
}

static inline bool row_ok_missing_next(uint8_t prev, uint8_t cur)  { return row_ok_generic(true,prev,false,0,cur); }

/* Special wrappers for the different boundary situations */
static inline bool row_ok_missing_prev(uint8_t cur, uint8_t next)  { return row_ok_generic(false,0,true,next,cur); }

static inline bool row_ok_no_adj      (uint8_t cur)               { return row_ok_generic(false,0,false,0,cur); }

static unsigned char trans[64][64][64];
static unsigned char trans_len[64][64];
static bool trans_done = false;

/* Build transition lists for interior rows where both neighbors exist. */
static void init_transitions(void)
{
    if (trans_done) return;
    for (uint8_t p = 0; p < 64; ++p) {
        for (uint8_t mid = 0; mid < 64; ++mid) {
            unsigned char k = 0;
            for (uint8_t nrow = 0; nrow < 64; ++nrow) {
                if (row_ok_generic(true, p, true, nrow, mid))
                    trans[p][mid][k++] = nrow;
            }
            trans_len[p][mid] = k;
        }
    }
    trans_done = true;
}

seq_t a(idx_t n)
{
    init_transitions();

    if (n == 0) return 0;          /* offset 1; not used but defensive */

    /* DP arrays: dp[prev][cur] holds number of ways ending with
       rows prev (before cur) and cur.                                            */
    static seq_t dp[64][64];
    static seq_t ndp[64][64];

    /* --- Handle n==1 separately ----------------------------------------- */
    if (n == 1) {
        seq_t res = 0;
        for (uint8_t r0 = 0; r0 < 64; ++r0)
            if ((r0 & 1) == 0 && row_ok_no_adj(r0))
                res += 1;
        return res;
    }

    /* --- Initialize dp with the first two rows --------------------------- */
    mem_set(dp, 0, sizeof(dp));
    for (uint8_t r0 = 0; r0 < 64; ++r0)
        if ((r0 & 1) == 0)                   /* upper‑left element zero   */
            for (uint8_t r1 = 0; r1 < 64; ++r1)
                if (row_ok_missing_prev(r0, r1))
                    dp[r0][r1] = 1;

    /* --- Handle n==2 ----------------------------------- */
    if (n == 2) {
        seq_t res = 0;
        for (uint8_t p = 0; p < 64; ++p)
            for (uint8_t c = 0; c < 64; ++c) {
                seq_t cnt = dp[p][c];
                if (cnt == 0) continue;
                if (row_ok_missing_next(p, c))
                    res += cnt;
            }
        return res;
    }

    /* --- For n > 2: iterate over remaining rows ------------------------ */
    idx_t remaining = n - 2;                /* number of rows to append after the first two */

    for (idx_t step = 0; step < remaining; ++step) {
        mem_set(ndp, 0, sizeof(ndp));
        for (uint8_t p = 0; p < 64; ++p) {
            for (uint8_t c = 0; c < 64; ++c) {
                seq_t cnt = dp[p][c];
                if (cnt == 0) continue;
                unsigned char k = trans_len[p][c];
                const uint8_t *list = trans[p][c];
                for (unsigned char t = 0; t < k; ++t) {
                    uint8_t nxt = list[t];
                    ndp[c][nxt] += cnt;
                }
            }
        }
        mem_copy(dp, ndp, sizeof(dp));
    }

    /* --- Final row validation ------------------------------------------- */
    seq_t res = 0;
    for (uint8_t p = 0; p < 64; ++p)
        for (uint8_t c = 0; c < 64; ++c) {
            seq_t cnt = dp[p][c];
            if (cnt == 0) continue;
            if (row_ok_missing_next(p, c))
                res += cnt;
        }
    return res;
}
