/*
 * A316799  Number of nX6 0..1 arrays with every element unequal to 2, 3, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A316799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP enumeration for A316799 */

#include <stdint.h>
#include <stdbool.h>

/* Check that for every cell in a 6-wide row `mid`, the number of
   king‑move adjacent cells that differ from the cell is not 0, 1, or 4.
   The neighbour rows are `above` (if any) and `below` (if any). */
static bool check_row(uint8_t above, uint8_t mid, uint8_t below,
                      bool has_above, bool has_below)
{
    for (int j = 0; j < 6; ++j) {
        int val = (mid >> j) & 1;
        int diff = 0;

        /* neighbours on the same row */
        if (j > 0)
            diff += ((((mid >> (j - 1)) & 1) != val));
        if (j < 5)
            diff += ((((mid >> (j + 1)) & 1) != val));

        /* neighbours in the row above */
        if (has_above) {
            if (j > 0)
                diff += ((((above >> (j - 1)) & 1) != val));
            diff += ((((above >> j) & 1) != val));
            if (j < 5)
                diff += ((((above >> (j + 1)) & 1) != val));
        }

        /* neighbours in the row below */
        if (has_below) {
            if (j > 0)
                diff += ((((below >> (j - 1)) & 1) != val));
            diff += ((((below >> j) & 1) != val));
            if (j < 5)
                diff += ((((below >> (j + 1)) & 1) != val));
        }

        /* disallow counts 0, 1, 4 */
        if (diff == 0 || diff == 1 || diff == 4)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Prebuilt tables – constructed once. */
    static bool built = false;
    static uint8_t trans_next[64][64][64];
    static uint8_t trans_count[64][64];
    static uint8_t start_next[64][64];
    static uint8_t start_count[64];
    static bool final_ok[64][64];
    static bool single_ok[64];

    if (!built) {
        built = true;

        /* Middle row transitions (need above, mid, below). */
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                int cnt = 0;
                for (int c = 0; c < 64; ++c) {
                    if (check_row((uint8_t)a, (uint8_t)b, (uint8_t)c,
                                  true, true))
                        trans_next[a][b][cnt++] = (uint8_t)c;
                }
                trans_count[a][b] = (uint8_t)cnt;
            }
        }

        /* First row transitions (no above). */
        for (int a = 0; a < 64; ++a) {
            int cnt = 0;
            for (int c = 0; c < 64; ++c) {
                if (check_row(0, (uint8_t)a, (uint8_t)c,
                              false, true))
                    start_next[a][cnt++] = (uint8_t)c;
            }
            start_count[a] = (uint8_t)cnt;
        }

        /* Final row check (no below). */
        for (int a = 0; a < 64; ++a)
            for (int b = 0; b < 64; ++b)
                final_ok[a][b] = check_row((uint8_t)a, (uint8_t)b, 0,
                                           true, false);

        /* Single‑row case (no neighbours). */
        for (int a = 0; a < 64; ++a)
            single_ok[a] = check_row(0, (uint8_t)a, 0,
                                     false, false);
    }

    /* n = 1 special case */
    if (n == 1) {
        seq_t ans = 0;
        for (int p = 0; p < 64; ++p) {
            if ((p & 1) != 0)   /* upper‑left must be 0 */
                continue;
            if (single_ok[p])
                ans += (seq_t)1;
        }
        return ans;
    }

    /* DP arrays: cur[a][b] = number of ways ending with rows (a,b). */
    static seq_t cur[64][64], nxt[64][64];

    /* initialise cur for the first two rows */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 64; ++j)
            cur[i][j] = 0;

    for (int p = 0; p < 64; ++p) {
        if ((p & 1) != 0)      /* upper‑left zero restriction */
            continue;
        uint8_t cnt = start_count[p];
        uint8_t *list = start_next[p];
        for (int k = 0; k < cnt; ++k) {
            uint8_t q = list[k];
            cur[p][q] = (seq_t)1;
        }
    }

    /* add rows 3 .. n */
    for (idx_t r = 2; r < n; ++r) {
        /* clear nxt */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                nxt[i][j] = 0;

        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                seq_t ways = cur[a][b];
                if (ways == 0)
                    continue;
                unsigned cnt = trans_count[a][b];
                uint8_t *list = trans_next[a][b];
                for (unsigned k = 0; k < cnt; ++k) {
                    uint8_t c = list[k];
                    nxt[b][c] += ways;
                }
            }
        }

        /* swap cur and nxt */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* sum over final rows that satisfy final_ok */
    seq_t ans = 0;
    for (int a = 0; a < 64; ++a)
        for (int b = 0; b < 64; ++b) {
            seq_t ways = cur[a][b];
            if (ways && final_ok[a][b])
                ans += ways;
        }

    return ans;
}
