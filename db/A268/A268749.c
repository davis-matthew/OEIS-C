/*
 * A268749  Number of nX7 binary arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling two no more
 *          than once.
 * https://oeis.org/A268749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with at most one pair of *
 *            horizontally or vertically adjacent 1s (i.e. total
 *            horizontal+vertical * adjacencies ≤ 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

typedef idx_t idx_t;   /* supplied by the build */
typedef seq_t seq_t;   /* supplied by the build */

/* Transition between two consecutive row masks.
 * h : number of horizontal adjacencies inside the next mask (0 or 1)
 * v : number of vertical adjacencies between previous and next mask (0 or 1) */
typedef struct {
    unsigned char mask;
    unsigned char h;
    unsigned char v;
} Trans;

static int hmask[128];
static Trans trans[128][128];
static unsigned char trans_cnt[128];
static int initialized = 0;

/* Pre‑compute horizontal‑pair counts for each mask and all allowed
 * transitions between masks. A transition is allowed iff it introduces
 * at most one horizontal pair in the new row *and* at most one vertical
 * pair with the previous row.  Masks that contain more than one
 * horizontal pair are discarded entirely. */
static void init_globals(void)
{
    int i, j;
    for (i = 0; i < 128; ++i) {
        /* count occurrences of '11' in the mask */
        hmask[i] = __builtin_popcount((unsigned int)(i & (i >> 1)));
    }

    for (i = 0; i < 128; ++i) {
        trans_cnt[i] = 0;
        for (j = 0; j < 128; ++j) {
            int h = hmask[j];
            if (h > 1) continue;                    /* mask itself too many horizontals */
            int v = __builtin_popcount((unsigned int)(i & j));
            if (v > 1) continue;                    /* too many verticals */
            int k = trans_cnt[i]++;
            trans[i][k].mask = (unsigned char)j;
            trans[i][k].h    = (unsigned char)h;
            trans[i][k].v    = (unsigned char)v;
        }
    }
    initialized = 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!initialized) init_globals();

    /* dp[mask][used] where 'used' = 0 → 0 adjacencies so far,
     *                    'used' = 1 → 1 adjacency already present. */
    seq_t dp[128][2];
    seq_t ndp[128][2];
    int i, j, k;

    /* initialise dp for the first row */
    for (i = 0; i < 128; ++i) {
        dp[i][0] = 0;
        dp[i][1] = 0;
    }
    for (i = 0; i < 128; ++i) {
        int h = hmask[i];
        if (h == 0)       dp[i][0] = 1;
        else if (h == 1)  dp[i][1] = 1;
        /* h > 1 never happens because those masks are omitted earlier */
    }

    if (n == 1) {
        seq_t total = 0;
        for (i = 0; i < 128; ++i) {
            total += dp[i][0];
            total += dp[i][1];
        }
        return total;
    }

    /* process remaining rows 2 … n */
    for (j = 2; j <= n; ++j) {
        /* zero initialise ndp */
        for (i = 0; i < 128; ++i) {
            ndp[i][0] = 0;
            ndp[i][1] = 0;
        }
        for (i = 0; i < 128; ++i) {
            seq_t val0 = dp[i][0];
            seq_t val1 = dp[i][1];
            if (val0 == 0 && val1 == 0) continue;
            int tcnt = trans_cnt[i];
            for (k = 0; k < tcnt; ++k) {
                const Trans *T = &trans[i][k];
                int total_edges = T->h + T->v;   /* adjacencies created by this transition */

                if (val0) {
                    if (total_edges == 0) ndp[T->mask][0] += val0;
                    else if (total_edges == 1) ndp[T->mask][1] += val0;
                }
                if (val1 && total_edges == 0) {
                    ndp[T->mask][1] += val1;
                }
            }
        }
        /* copy ndp → dp (explicit assignment, not memcpy) */
        for (i = 0; i < 128; ++i) {
            dp[i][0] = ndp[i][0];
            dp[i][1] = ndp[i][1];
        }
    }

    /* sum all valid states after processing n rows */
    seq_t answer = 0;
    for (i = 0; i < 128; ++i) {
        answer += dp[i][0];
        answer += dp[i][1];
    }
    return answer;
}
