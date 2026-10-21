/*
 * A250838  Number of (n+1)X(5+1) 0..2 arrays with no 2X2 subblock having a
 *          diagonal absolute difference less than its antidiagonal absolute
 *          difference.
 * https://oeis.org/A250838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A250838 – number of (n+1)×6 arrays with no 2×2 subblock having
   a diagonal absolute difference less than its anti‑diagonal absolute
   difference.  The code uses a transfer‑matrix DP over all 3⁶ possible
   rows.  The adjacency condition is:
      for every j = 0..4,
      |row1[j] – row2[j+1]| ≥ |row1[j+1] – row2[j]|.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define STS 729                 /* 3⁶ possible rows */

static bool init_done = false;
/* digits of each row: row_digits[r][col] with col = 0..5 */
static int row_digits[STS][6];
/* number of neighbours for each row */
static int neighbor_cnt[STS];
/* neighbour lists: neighbors[r] points to an array of length neighbor_cnt[r] */
static int *neighbors[STS];

/* Check whether two rows r and s are compatible (no forbidden 2×2 subblock). */
static bool compatible(int r, int s)
{
    int j;
    for (j = 0; j < 5; ++j) {
        int a1 = row_digits[r][j];
        int a2 = row_digits[r][j + 1];
        int b1 = row_digits[s][j];
        int b2 = row_digits[s][j + 1];
        int d1 = a1 - b2; if (d1 < 0) d1 = -d1;   /* diagonal difference */
        int d2 = a2 - b1; if (d2 < 0) d2 = -d2;   /* anti‑diagonal difference */
        /* forbidden if diagonal difference is strictly smaller */
        if (d1 < d2) return false;
    }
    return true;
}

/* One‑time initialization of row encodings and neighbour graph. */
static void init(void)
{
    /* encode all rows in base‑3 */
    for (int r = 0; r < STS; ++r) {
        int tmp = r;
        for (int k = 5; k >= 0; --k) {
            row_digits[r][k] = tmp % 3;
            tmp /= 3;
        }
    }

    /* compute neighbour counts and allocate lists */
    for (int r = 0; r < STS; ++r) {
        int cnt = 0;
        for (int s = 0; s < STS; ++s)
            if (compatible(r, s))
                ++cnt;
        neighbor_cnt[r] = cnt;
        if (cnt > 0)
            neighbors[r] = (int *)malloc(cnt * sizeof(int));
        else
            neighbors[r] = NULL;
    }

    /* fill neighbour lists */
    for (int r = 0; r < STS; ++r) {
        int pos = 0;
        for (int s = 0; s < STS; ++s)
            if (compatible(r, s))
                neighbors[r][pos++] = s;
    }

    init_done = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* sequence is defined for n ≥ 1 */

    if (!init_done)
        init();

    seq_t cur[STS];
    seq_t nxt[STS];

    /* each state is reachable by one initial row */
    for (int i = 0; i < STS; ++i)
        cur[i] = 1;

    /* add rows one by one – step equals number of added rows = n */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STS; ++i)
            nxt[i] = 0;

        for (int i = 0; i < STS; ++i) {
            if (cur[i] == 0)
                continue;
            seq_t cnt = cur[i];
            int nc = neighbor_cnt[i];
            for (int k = 0; k < nc; ++k) {
                int j = neighbors[i][k];
                nxt[j] += cnt;
            }
        }

        for (int i = 0; i < STS; ++i)
            cur[i] = nxt[i];
    }

    /* sum over all ending rows */
    seq_t ans = 0;
    for (int i = 0; i < STS; ++i)
        ans += cur[i];

    return ans;
}
