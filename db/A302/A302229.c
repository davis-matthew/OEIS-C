/*
 * A302229  Number of 6Xn 0..1 arrays with every element equal to 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic‑programming over columns for
   6×n binary arrays where each cell has 1–3 neighbours
   of the same value among horizontal and diagonal (no vertical) adjacent cells.
   The upper‑left element is forced to 0.  This uses a 6‑bit mask for each
   column; state consists of the last two columns. */

#include <stdint.h>
#include <stdbool.h>

/* Check if a column satisfies the neighbour count constraint
   given its left and right neighbours.  Vertical neighbours are NOT
   counted according to the definition. */
static inline bool valid_column(uint64_t col,
                                bool left_exists, uint64_t left,
                                bool right_exists, uint64_t right)
{
    for (int r = 0; r < 6; ++r) {
        uint8_t v = (col >> r) & 1;
        uint8_t cnt = 0;

        if (left_exists) {
            /* horizontal neighbour from the left column */
            if (((left >> r) & 1) == v) ++cnt;
            /* diagonal neighbours from the left column */
            if (r > 0 && (((left >> (r - 1)) & 1) == v)) ++cnt;
            if (r < 5 && (((left >> (r + 1)) & 1) == v)) ++cnt;
        }

        if (right_exists) {
            if (((right >> r) & 1) == v) ++cnt;
            if (r > 0 && (((right >> (r - 1)) & 1) == v)) ++cnt;
            if (r < 5 && (((right >> (r + 1)) & 1) == v)) ++cnt;
        }

        /* Vertical neighbours are explicitly excluded. */

        if (cnt < 1 || cnt > 3)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1) return 0;

    /* Precomputed transition masks: for each left L and middle M,
       the set of right columns N that make M valid with both neighbours. */
    static unsigned long long trans[64][64];
    static bool ready = false;
    if (!ready) {
        for (int L = 0; L < 64; ++L) {
            for (int M = 0; M < 64; ++M) {
                unsigned long long mask = 0;
                for (int N = 0; N < 64; ++N) {
                    if (valid_column((uint64_t)M,
                                     true, (uint64_t)L,
                                     true, (uint64_t)N))
                        mask |= 1ULL << N;
                }
                trans[L][M] = mask;
            }
        }
        ready = true;
    }

    seq_t dp_cur[64][64] = { 0 };
    seq_t dp_next[64][64];

    /* Initialise with the first two columns.
       The upper‑left element (row 0, col 0) must be zero. */
    for (int col0 = 0; col0 < 64; ++col0) {
        if ((col0 & 1) != 0) continue;            /* upper‑left must be 0 */
        for (int col1 = 0; col1 < 64; ++col1) {
            if (valid_column((uint64_t)col0,
                             false, 0,
                             true, (uint64_t)col1))
                dp_cur[col0][col1] += 1;
        }
    }

    /* Extend the array column by column.  For n = 2 the loop is skipped. */
    for (idx_t step = 2; step < n; ++step) {
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev < 64; ++prev) {
            for (int curr = 0; curr < 64; ++curr) {
                seq_t curVal = dp_cur[prev][curr];
                if (!curVal) continue;
                unsigned long long nxtMask = trans[prev][curr];
                while (nxtMask) {
                    unsigned long long bit = nxtMask & -nxtMask;
                    int next = __builtin_ctzll(bit);
                    dp_next[curr][next] += curVal;
                    nxtMask ^= bit;
                }
            }
        }

        /* swap dp_next into dp_cur */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 64; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    /* Final column has no neighbour to the right.  Sum all valid endings. */
    seq_t ans = 0;
    for (int prev = 0; prev < 64; ++prev) {
        for (int curr = 0; curr < 64; ++curr) {
            seq_t val = dp_cur[prev][curr];
            if (!val) continue;
            if (valid_column((uint64_t)curr,
                             true, (uint64_t)prev,
                             false, 0))
                ans += val;
        }
    }

    return ans;
}
