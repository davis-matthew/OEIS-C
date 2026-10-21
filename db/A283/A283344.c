/*
 * A283344  Number of nX5 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS A283344.
   a(n) = number of n × 5 binary arrays in which exactly one 1 has more
   than two of its horizontal, vertical, or antidiagonal neighbors.
   The implementation uses dynamic programming over the last two rows.
*/
#include <stdint.h>
#include <string.h>

static unsigned char pre[32][32][32];   /* violation count for middle row */

/* Helper to build the pre table */
static void build_pre(void)
{
    int prevprev, prev, cur, col;
    for (prevprev = 0; prevprev < 32; ++prevprev) {
        for (prev = 0; prev < 32; ++prev) {
            for (cur = 0; cur < 32; ++cur) {
                int cnt = 0;
                for (col = 0; col < 5; ++col) {
                    if (!((prev >> col) & 1))
                        continue; /* only consider 1's */
                    int neigh = 0;
                    if (col > 0 && ((prev >> (col - 1)) & 1)) ++neigh; /* left */
                    if (col < 4 && ((prev >> (col + 1)) & 1)) ++neigh; /* right */
                    if ((prevprev >> col) & 1) ++neigh;                /* up */
                    if ((cur >> col) & 1) ++neigh;                    /* down */
                    if (col > 0 && ((prevprev >> (col - 1)) & 1)) ++neigh; /* NE (up-left) */
                    if (col < 4 && ((cur >> (col + 1)) & 1)) ++neigh;       /* SW (down-right) */
                    if (neigh > 2)
                        ++cnt;
                }
                pre[prevprev][prev][cur] = (unsigned char)cnt;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Build the pre table once */
    static int pre_built = 0;
    if (!pre_built) {
        build_pre();
        pre_built = 1;
    }

    /* DP arrays: 32*32*2 = 2048 states */
    const size_t STATES = 2048;
    seq_t *cur  = (seq_t *)calloc(STATES, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(STATES, sizeof(seq_t));
    if (!cur || !next) return 0; /* allocation failure */

    /* Initial state: only one row processed (row 0), prevprev = 0 */
    for (idx_t pattern = 0; pattern < 32; ++pattern) {
        size_t idx = (((size_t)0 << 5) | pattern) << 1; /* flag = 0 */
        cur[idx] = 1;
    }

    /* Process rows 2 .. n */
    for (idx_t r = 2; r <= n; ++r) {
        memset(next, 0, STATES * sizeof(seq_t));
        for (size_t st = 0; st < STATES; ++st) {
            seq_t val = cur[st];
            if (!val)
                continue;
            int flag = st & 1;
            size_t base = st >> 1;
            int prevprev = (int)(base >> 5);
            int prev     = (int)(base & 31);
            for (int currow = 0; currow < 32; ++currow) {
                int viol = pre[prevprev][prev][currow];
                if (viol > 1)
                    continue;
                int newflag = flag;
                if (viol == 1) {
                    if (flag)
                        continue;
                    newflag = 1;
                }
                size_t newbase = ((size_t)prev << 5) | currow;
                size_t newst = (newbase << 1) | newflag;
                next[newst] += val;
            }
        }
        /* swap cur and next */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Final evaluation of the last row: next row is zero pattern */
    seq_t ans = 0;
    for (size_t st = 0; st < STATES; ++st) {
        seq_t val = cur[st];
        if (!val)
            continue;
        int flag = st & 1;
        size_t base = st >> 1;
        int prevprev = (int)(base >> 5);
        int prev     = (int)(base & 31);
        int viol = pre[prevprev][prev][0]; /* next row = 0 */
        if (viol > 1)
            continue;
        int final_flag = flag;
        if (viol == 1) {
            if (flag)
                continue;
            final_flag = 1;
        }
        if (final_flag == 1)
            ans += val;
    }

    free(cur);
    free(next);
    return ans;
}
