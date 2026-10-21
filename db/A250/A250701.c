/*
 * A250701  Number of (n+1)X(2+1) 0..3 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries in 0..3 that
 *            satisfy 1. For each j=1,2 the horizontal sum x(i,j)+x(i,j-1) is
 *            nondecreasing in i. 2. For each i≥1 the vertical mins
 *            min(x(i,j),x(i-1,j)) are nondecreasing in j. Computation uses a
 *            64‑state DP
 * Range:     u8 none, u16 n <= 3, u32 n <= 12, u64 n <= 39, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#define STATE_COUNT 64

static void build_transitions(bool transition[STATE_COUNT][STATE_COUNT],
                              int *cnt)
{
    for (int a = 0; a < 4; ++a)
        for (int b = 0; b < 4; ++b)
            for (int c = 0; c < 4; ++c) {
                int s = (a * 4 + b) * 4 + c;
                int s1_s = a + b;
                int s2_s = b + c;
                cnt[s] = 0;
                for (int x = 0; x < 4; ++x)
                    for (int y = 0; y < 4; ++y)
                        for (int z = 0; z < 4; ++z) {
                            int t = (x * 4 + y) * 4 + z;
                            int s1_t = x + y;
                            int s2_t = y + z;
                            if (s1_t < s1_s || s2_t < s2_s)
                                continue;
                            int m0 = (x < a) ? x : a;
                            int m1 = (y < b) ? y : b;
                            int m2 = (z < c) ? z : c;
                            if (m0 > m1 || m1 > m2)
                                continue;
                            transition[s][t] = true;
                            ++cnt[s];
                        }
            }
}

seq_t a(idx_t n)
{
    static bool built = false;
    static bool trans[STATE_COUNT][STATE_COUNT];
    static int trans_count[STATE_COUNT];
    if (!built) {
        for (int i = 0; i < STATE_COUNT; ++i)
            for (int j = 0; j < STATE_COUNT; ++j)
                trans[i][j] = false;
        build_transitions(trans, trans_count);
        built = true;
    }

    // DP arrays
    seq_t *cur = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    seq_t *next = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    if (!cur || !next) {
        // allocation failure: return 0
        if (cur) free(cur);
        if (next) free(next);
        return 0;
    }

    for (int s = 0; s < STATE_COUNT; ++s)
        cur[s] = 1;   // first row is arbitrary

    for (idx_t step = 1; step <= n; ++step) {
        for (int t = 0; t < STATE_COUNT; ++t)
            next[t] = 0;
        for (int s = 0; s < STATE_COUNT; ++s) {
            if (cur[s] == 0) continue;
            seq_t val = cur[s];
            for (int t = 0; t < STATE_COUNT; ++t) {
                if (!trans[s][t]) continue;
                next[t] += val;
            }
        }
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t ans = 0;
    for (int s = 0; s < STATE_COUNT; ++s)
        ans += cur[s];

    free(cur);
    free(next);
    return ans;
}
