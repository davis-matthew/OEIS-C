/*
 * A250135  Number of length n+2 0..3 arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 43, u128 n <= 93,
 *            big any n
 * Helpers:   _median3, _tidx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250135: Number of length n+2 0..3 arrays with the medians of every
 * three consecutive terms nondecreasing.
 * The values are computed by dynamic programming over all triples.
 * In particular:
 *   a(1)   = 64
 *   a(2)   = 204
 *   a(3)   = 632
 *   a(4)   = 1959
 *   a(5)   = 5696
 *   a(6)   = 16104
 *   a(7)   = 45232
 *   ...
 * (the recurrence is linear in the number of triples).
 */
#include <stdbool.h>
#include "../utils/_median3.h"
#include "../utils/_tidx.h"

static bool _adj_built = false;
static int  _neigh_cnt[64];
static int  _neigh[64][4];

/* build transition lists: for every triple (a,b,c) keep all
 * triples (b,c,d) such that median(a,b,c) <= median(b,c,d)     */
static void _build_adj(void)
{
    for (int a = 0; a < 4; ++a)
    for (int b = 0; b < 4; ++b)
    for (int c = 0; c < 4; ++c) {
        int i = _tidx(a,b,c);
        int m1 = _median3(a,b,c);
        int k = 0;
        for (int d = 0; d < 4; ++d) {
            int m2 = _median3(b,c,d);
            if (m1 <= m2) {
                _neigh[i][k++] = _tidx(b,c,d);
            }
        }
        _neigh_cnt[i] = k;
    }
    _adj_built = true;
}

seq_t a(idx_t n)
{
    /* build adjacency once */
    if (!_adj_built) _build_adj();

    /* DP arrays over 64 states (last 3 elements) */
    static seq_t dp[64];
    static seq_t nextdp[64];
    seq_t result = 0;

    /* initial states: every triple appears exactly once as the first 3
     * elements of the array, so each state gets weight 1. */
    for (int i = 0; i < 64; ++i) dp[i] = 1;

    /* number of transition steps: for an array of length n+2 we
     * add n-1 further elements after the first triple. */
    idx_t steps = (n > 1) ? n - 1 : 0;

    for (idx_t step = 0; step < steps; ++step) {
        /* clear nextdp */
        for (int i = 0; i < 64; ++i) nextdp[i] = 0;

        /* propagate transitions */
        for (int i = 0; i < 64; ++i) {
            seq_t cur = dp[i];
            if (!cur) continue;
            int cnt = _neigh_cnt[i];
            for (int k = 0; k < cnt; ++k) {
                int j = _neigh[i][k];
                nextdp[j] += cur;
            }
        }

        /* copy back */
        for (int i = 0; i < 64; ++i) dp[i] = nextdp[i];
    }

    /* sum all states – each corresponds to a completed array */
    for (int i = 0; i < 64; ++i) result += dp[i];
    return result;
}
