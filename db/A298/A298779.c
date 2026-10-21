/*
 * A298779  Number of nX5 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 33, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is computed by dynamic programming over 5‑bit row patterns.
 * For each n, we count the number of n×5 binary arrays such that
 * 1. the cell (0,0) is 0,
 * 2. for every cell, the number of adjacent cells (king moves) that have
 *    the same value is in the set {0,3,4,5,8}.
 * The DP uses 2^5 = 32 possible row states; a transition is allowed
 * iff the middle row satisfies the condition with its two neighbours.
 */

#include <stdbool.h>
#include <string.h>
#include <stdint.h>        /* for uint8_t, uint8_t arrays            */

#define WIDTH 5
#define STATES (1 << WIDTH)

/* allowed neighbour counts: 0,3,4,5,8 are permitted */
static const bool allowed[9] = {
    true,   /* 0 */
   false,   /* 1 */
   false,   /* 2 */
    true,    /* 3 */
    true,    /* 4 */
    true,    /* 5 */
   false,    /* 6 */
   false,    /* 7 */
    true     /* 8 */
};

/* Transition tables */
static bool validTop[STATES][STATES];
static bool validBottom[STATES][STATES];
static bool validMiddle[STATES][STATES][STATES];
static unsigned char nextCnt[STATES][STATES];
static int nextState[STATES][STATES][STATES];
static bool tables_computed = false;

/* Count neighbours that have the same value as the cell in 'cur' at column 'col'.
 * 'above' and 'below' are the neighbouring rows. If has_above or has_below
 * is false, the corresponding neighbours are ignored. */
static int same_value_neighbours(int above, int cur, int below,
                                 int col, bool has_above, bool has_below)
{
    int curr_bit = (cur >> col) & 1;
    int cnt = 0;

    if (has_above) {
        if (col > 0 && ((above >> (col - 1)) & 1) == curr_bit) cnt++;   /* TL */
        if (((above >> col) & 1) == curr_bit) cnt++;                     /* T  */
        if (col < WIDTH - 1 && ((above >> (col + 1)) & 1) == curr_bit) cnt++; /* TR */
    }
    if (has_below) {
        if (col > 0 && ((below >> (col - 1)) & 1) == curr_bit) cnt++;   /* BL */
        if (((below >> col) & 1) == curr_bit) cnt++;                     /* B  */
        if (col < WIDTH - 1 && ((below >> (col + 1)) & 1) == curr_bit) cnt++; /* BR */
    }
    if (col > 0 && ((cur >> (col - 1)) & 1) == curr_bit) cnt++;          /* L  */
    if (col < WIDTH - 1 && ((cur >> (col + 1)) & 1) == curr_bit) cnt++;   /* R  */

    return cnt;
}

/* Pre‑compute transition tables.  This is done only once. */
static void compute_tables(void)
{
    if (tables_computed) return;

    for (int a = 0; a < STATES; ++a) {
        for (int b = 0; b < STATES; ++b) {
            /* Top row (no above neighbour) */
            bool ok = true;
            for (int c = 0; c < WIDTH && ok; ++c) {
                int cnt = same_value_neighbours(0, a, b, c, false, true);
                if (!allowed[cnt]) ok = false;
            }
            validTop[a][b] = ok;

            /* Bottom row (no below neighbour) */
            ok = true;
            for (int c = 0; c < WIDTH && ok; ++c) {
                int cnt = same_value_neighbours(a, b, 0, c, true, false);
                if (!allowed[cnt]) ok = false;
            }
            validBottom[a][b] = ok;

            /* Middle rows (has both neighbours) */
            int pos = 0;
            for (int c = 0; c < STATES; ++c) {
                bool ok_mid = true;
                for (int d = 0; d < WIDTH && ok_mid; ++d) {
                    int cnt = same_value_neighbours(a, b, c, d, true, true);
                    if (!allowed[cnt]) ok_mid = false;
                }
                validMiddle[a][b][c] = ok_mid;
                if (ok_mid) {
                    nextState[a][b][pos++] = c;
                }
            }
            nextCnt[a][b] = (unsigned char)pos;
        }
    }
    tables_computed = true;
}

/* Single row (no vertical neighbours) is valid iff each cell's
 * same‑value horizontal neighbours count is allowed. */
static bool single_row_valid(int row)
{
    for (int col = 0; col < WIDTH; ++col) {
        int bit = (row >> col) & 1;
        int cnt = 0;
        if (col > 0 && ((row >> (col - 1)) & 1) == bit) cnt++;
        if (col < WIDTH - 1 && ((row >> (col + 1)) & 1) == bit) cnt++;
        if (!allowed[cnt]) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */
/* Public sequence function                                            */
/* -------------------------------------------------------------------- */
extern seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if (n == 1) {                     /* treat the single row case */
        seq_t sum = 0;
        for (int r = 0; r < STATES; ++r) {
            if (r & 1) continue;       /* upper‑left must be 0 */
            if (single_row_valid(r)) sum += (seq_t)1;
        }
        return sum;
    }

    compute_tables();

    /* DP arrays: cur[prev][cur] = number of ways to reach this pair */
    static seq_t cur[STATES][STATES];
    static seq_t nxt[STATES][STATES];
    memset(cur, 0, sizeof(cur));

    /* Initialise with first two rows */
    for (int r0 = 0; r0 < STATES; ++r0) {
        if (r0 & 1) continue;                      /* upper‑left zero */
        for (int r1 = 0; r1 < STATES; ++r1) {
            if (validTop[r0][r1]) cur[r0][r1] += (seq_t)1;
        }
    }

    /* Process remaining rows 3..n (index 2..n-1) */
    for (idx_t row = 2; row < n; ++row) {
        memset(nxt, 0, sizeof(nxt));
        for (int a = 0; a < STATES; ++a) {
            for (int b = 0; b < STATES; ++b) {
                seq_t ways = cur[a][b];
                if (!ways) continue;
                unsigned char cnt = nextCnt[a][b];
                for (int k = 0; k < cnt; ++k) {
                    int c = nextState[a][b][k];
                    nxt[b][c] += ways;
                }
            }
        }
        memcpy(cur, nxt, sizeof(cur));
    }

    /* Final bottom validation */
    seq_t result = 0;
    for (int prev = 0; prev < STATES; ++prev) {
        for (int last = 0; last < STATES; ++last) {
            seq_t ways = cur[prev][last];
            if (!ways) continue;
            if (validBottom[prev][last]) result += ways;
        }
    }
    return result;
}
