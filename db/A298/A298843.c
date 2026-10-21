/*
 * A298843  Number of nX5 0..1 arrays with every element equal to 1, 2, 3, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming over rows for each n */
/* The algorithm enumerates all 5‑column binary patterns for each row and
   propagates constraints that the number of equal‑valued king‑neighbour
   cells must be one of {1,2,3,6,8}.  The first cell (upper‑left)
   is forced to be 0.  This implementation works for any build
   where seq_t and idx_t are provided. */

#include <stdbool.h>
#include <stdint.h>

/* number of columns (constant given in the definition) */
#define COLS 5
#define ROWS (1 << COLS)        /* 32 possible row patterns */
#define MISSING 32               /* sentinel for a missing top row */
#define STATE_ROWS (ROWS + 1)    /* 0..31 real rows, 32 = MISSING */

/* adjacency rule – which neighbour counts are allowed */
static const bool allowed[9] = {
    false, /* 0 */
    true,  /* 1 */
    true,  /* 2 */
    true,  /* 3 */
    false, /* 4 */
    false, /* 5 */
    true,  /* 6 */
    false, /* 7 */
    true   /* 8 */
};

/* ----- Helper functions --------------------------------------------------- */
static bool good_internal(int prevRow, int currRow, int nextRow)
{
    for (int col = 0; col < COLS; ++col) {
        int val = (currRow >> col) & 1;
        int cnt = 0;
        /* up neighbours (prevRow) */
        if (col > 0   && ((prevRow >> (col - 1)) & 1) == val) ++cnt;
        if (          ((prevRow >> col)      & 1) == val) ++cnt;
        if (col < COLS-1 && ((prevRow >> (col + 1)) & 1) == val) ++cnt;
        /* same row neighbours */
        if (col > 0   && ((currRow >> (col - 1)) & 1) == val) ++cnt;
        if (col < COLS-1 && ((currRow >> (col + 1)) & 1) == val) ++cnt;
        /* down neighbours (nextRow) */
        if (col > 0   && ((nextRow >> (col - 1)) & 1) == val) ++cnt;
        if (          ((nextRow >> col)      & 1) == val) ++cnt;
        if (col < COLS-1 && ((nextRow >> (col + 1)) & 1) == val) ++cnt;
        if (!allowed[cnt]) return false;
    }
    return true;
}

static bool good_top(int currRow, int nextRow)
{
    for (int col = 0; col < COLS; ++col) {
        int val = (currRow >> col) & 1;
        int cnt = 0;
        /* same row neighbours */
        if (col > 0   && ((currRow >> (col - 1)) & 1) == val) ++cnt;
        if (col < COLS-1 && ((currRow >> (col + 1)) & 1) == val) ++cnt;
        /* down neighbours (nextRow) */
        if (col > 0   && ((nextRow >> (col - 1)) & 1) == val) ++cnt;
        if (          ((nextRow >> col)      & 1) == val) ++cnt;
        if (col < COLS-1 && ((nextRow >> (col + 1)) & 1) == val) ++cnt;
        if (!allowed[cnt]) return false;
    }
    return true;
}

static bool good_bottom(int prevRow, int currRow)
{
    for (int col = 0; col < COLS; ++col) {
        int val = (currRow >> col) & 1;
        int cnt = 0;
        /* up neighbours (prevRow) */
        if (col > 0   && ((prevRow >> (col - 1)) & 1) == val) ++cnt;
        if (          ((prevRow >> col)      & 1) == val) ++cnt;
        if (col < COLS-1 && ((prevRow >> (col + 1)) & 1) == val) ++cnt;
        /* same row neighbours */
        if (col > 0   && ((currRow >> (col - 1)) & 1) == val) ++cnt;
        if (col < COLS-1 && ((currRow >> (col + 1)) & 1) == val) ++cnt;
        if (!allowed[cnt]) return false;
    }
    return true;
}

static bool good_single(int currRow)
{
    for (int col = 0; col < COLS; ++col) {
        int val = (currRow >> col) & 1;
        int cnt = 0;
        /* same row neighbours only */
        if (col > 0   && ((currRow >> (col - 1)) & 1) == val) ++cnt;
        if (col < COLS-1 && ((currRow >> (col + 1)) & 1) == val) ++cnt;
        if (!allowed[cnt]) return false;
    }
    return true;
}

/* ----- Precomputation data ---------------------------------------------- */
typedef struct {
    uint8_t c;      /* number of successors */
    uint8_t nxt[ROWS];
} adj_t;

/* static tables of adjacency and validity */
static bool initialized = false;
static adj_t adj[STATE_ROWS * ROWS];
static bool bottom_val[ROWS][ROWS];
static bool single_val[ROWS];

static void init_tables(void)
{
    if (initialized) return;
    initialized = true;

    /* build adjacency lists for all (prevRow, currRow) pairs */
    for (int prev = 0; prev <= MISSING; ++prev) {
        for (int curr = 0; curr < ROWS; ++curr) {
            int idx = prev * ROWS + curr;
            adj[idx].c = 0;
            for (int nxt = 0; nxt < ROWS; ++nxt) {
                bool ok;
                if (prev == MISSING)
                    ok = good_top(curr, nxt);
                else
                    ok = good_internal(prev, curr, nxt);
                if (ok) {
                    adj[idx].nxt[adj[idx].c++] = (uint8_t)nxt;
                }
            }
        }
    }

    /* bottom row validity */
    for (int prev = 0; prev < ROWS; ++prev)
        for (int curr = 0; curr < ROWS; ++curr)
            bottom_val[prev][curr] = good_bottom(prev, curr);

    /* single row validity (no up/down neighbours) */
    for (int curr = 0; curr < ROWS; ++curr)
        single_val[curr] = good_single(curr);
}

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_tables();

    /* DP arrays: STATE_ROWS (prev) x ROWS (curr) */
    static seq_t dp1[STATE_ROWS][ROWS];
    static seq_t dp2[STATE_ROWS][ROWS];
    seq_t (*dp_cur)[ROWS] = dp1;
    seq_t (*dp_next)[ROWS] = dp2;

    /* zero dp_cur */
    for (int i = 0; i < STATE_ROWS; ++i)
        for (int j = 0; j < ROWS; ++j)
            dp_cur[i][j] = 0;

    /* first row (prev = MISSING), only upper‑left element must be 0 */
    for (int pat = 0; pat < ROWS; ++pat) {
        if ((pat & 1) == 0)          /* upper‑left element zero */
            dp_cur[MISSING][pat] = 1;
    }

    /* process remaining rows */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp_next */
        for (int i = 0; i < STATE_ROWS; ++i)
            for (int j = 0; j < ROWS; ++j)
                dp_next[i][j] = 0;

        for (int prev = 0; prev <= MISSING; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr) {
                seq_t val = dp_cur[prev][curr];
                if (!val) continue;
                const adj_t *ar = &adj[prev * ROWS + curr];
                for (uint8_t k = 0; k < ar->c; ++k) {
                    int nxt = ar->nxt[k];
                    dp_next[curr][nxt] += val;
                }
            }
        }
        /* swap dp_cur and dp_next */
        seq_t (*tmp)[ROWS] = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* final counting */
    seq_t result = 0;
    if (n == 1) {
        for (int curr = 0; curr < ROWS; ++curr)
            if (dp_cur[MISSING][curr] && single_val[curr])
                result += dp_cur[MISSING][curr];
    } else {
        for (int prev = 0; prev < ROWS; ++prev) {
            for (int curr = 0; curr < ROWS; ++curr)
                if (dp_cur[prev][curr] && bottom_val[prev][curr])
                    result += dp_cur[prev][curr];
        }
    }
    return result;
}
