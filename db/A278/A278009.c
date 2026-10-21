/*
 * A278009  Number of nX3 0..2 arrays with every element plus 1 mod 3 equal to
 *          some element at offset (-1,-1) (-1,0) (-1,1) (0,-1) (0,1) or
 *          (1,0), with upper left element zero.
 * https://oeis.org/A278009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278009: Number of n×3 arrays with the local +1 mod 3 constraint,
   upper‑left element fixed to 0.  Computation is performed by
   dynamic programming over pairs of rows.  For each pair (p,q) of
   consecutive rows we know whether the middle row p satisfies its
   constraints once the next row is fixed; the final row is checked
   after all transitions. */

#include <string.h>
#include <stdbool.h>

/* helper struct for adjacency lists */
typedef struct {
    unsigned char cnt;      /* number of admissible next rows */
    unsigned char next[27]; /* the admissible next row indices */
} Cmp;

/* 27 row patterns, encoded as base‑3 digits: col 0,1,2 */
static int patterns[27][3];
/* for each pair of rows (up,cur) the admissible down rows */
static Cmp next_list[27][27];
/* flag to initialise data once */
static bool init_done = false;

/* check if the "cur" row satisfies its constraint with given
   up and down rows.  pass -1 for missing rows. */
static bool check_row_idx(int up, int cur, int down)
{
    for (int j = 0; j < 3; ++j) {
        int v = patterns[cur][j];
        int target = (v + 1) % 3;
        bool ok = false;
        if (!ok && up >= 0 && j > 0 && patterns[up][j - 1] == target) ok = true;
        if (!ok && up >= 0 && patterns[up][j] == target)        ok = true;
        if (!ok && up >= 0 && j < 2 && patterns[up][j + 1] == target) ok = true;
        if (!ok && j > 0 && patterns[cur][j - 1] == target)        ok = true;
        if (!ok && j < 2 && patterns[cur][j + 1] == target)        ok = true;
        if (!ok && down >= 0 && patterns[down][j] == target)      ok = true;
        if (!ok) return false;
    }
    return true;
}

/* initialise patterns and adjacency lists */
static void init(void)
{
    if (init_done) return;
    /* generate all 27 patterns */
    for (int p = 0; p < 27; ++p) {
        patterns[p][0] = (p / 9) % 3;    /* leftmost column */
        patterns[p][1] = (p / 3) % 3;
        patterns[p][2] = p % 3;
    }
    /* pre‑compute adjacency lists */
    for (int up = 0; up < 27; ++up) {
        for (int cur = 0; cur < 27; ++cur) {
            Cmp *list = &next_list[up][cur];
            list->cnt = 0;
            for (int down = 0; down < 27; ++down) {
                if (check_row_idx(up, cur, down)) {
                    list->next[list->cnt++] = (unsigned char)down;
                }
            }
        }
    }
    init_done = true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init();
    /* counts for pairs of rows */
    static seq_t dp_prev[27][27];
    static seq_t dp_next[27][27];
    seq_t result = 0;

    /* n == 1 */
    if (n == 1) {
        for (int p = 0; p < 27; ++p) {
            if (patterns[p][0] != 0) continue;            /* upper‑left must be 0 */
            if (check_row_idx(-1, p, -1)) result += (seq_t)1;
        }
        return result;
    }

    /* initialize dp_prev: all pairs (first,second) with first row constraint satisfied */
    memset(dp_prev, 0, sizeof(dp_prev));
    for (int first = 0; first < 27; ++first) {
        if (patterns[first][0] != 0) continue;               /* (0,0) fixed */
        for (int second = 0; second < 27; ++second) {
            if (check_row_idx(-1, first, second))
                dp_prev[first][second] += (seq_t)1;
        }
    }

    /* iterate rows 3..n */
    for (int i = 3; (idx_t)i <= n; ++i) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int up = 0; up < 27; ++up) {
            for (int cur = 0; cur < 27; ++cur) {
                seq_t val = dp_prev[up][cur];
                if (!val) continue;
                const Cmp *list = &next_list[up][cur];
                for (int k = 0; k < list->cnt; ++k) {
                    int down = list->next[k];
                    dp_next[cur][down] += val;
                }
            }
        }
        /* copy dp_next to dp_prev for next iteration */
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
    }

    /* finally sum over states where the last row satisfies its constraint */
    for (int up = 0; up < 27; ++up) {
        for (int cur = 0; cur < 27; ++cur) {
            seq_t val = dp_prev[up][cur];
            if (!val) continue;
            if (check_row_idx(up, cur, -1))
                result += val;
        }
    }
    return result;
}
