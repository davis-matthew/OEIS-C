/*
 * A249705  Number of length n+3 0..6 arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+3 arrays over {0..6} such that for
 *            every four consecutive terms the maximum of some two terms
 *            equals the minimum of the remaining two terms
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses dynamic programming on the last
   three values of the array.  The state space has 7³ = 343 states
   corresponding to all possible triples of last three elements.
   For each state and possible next element we precompute whether
   the resulting 4‑tuple satisfies the local condition.  Then
   we iterate n times adding one element at a time.     */

#include <stdbool.h>
#include <string.h>   /* for memset */
#include <stdint.h>   /* for uint64_t etc */

/* Mapping from triple (x,y,z) to index 0..342 and back.
   These tables are built once upon the first call. */
static bool          valid[7][7][7][7] = {0};
static bool          tables_built      = false;
static int           idx_map[7][7][7];
static int           triple_arr[343][3];

static void build_tables(void)
{
    if (tables_built) return;
    tables_built = true;

    /* Build idx_map and triple_arr */
    int k = 0;
    for (int x = 0; x < 7; ++x)
        for (int y = 0; y < 7; ++y)
            for (int z = 0; z < 7; ++z) {
                triple_arr[k][0] = x;
                triple_arr[k][1] = y;
                triple_arr[k][2] = z;
                idx_map[x][y][z] = k;
                ++k;
            }

    /* Precompute validity of every 4‑tuple */
    for (int a = 0; a < 7; ++a)
        for (int b = 0; b < 7; ++b)
            for (int c = 0; c < 7; ++c)
                for (int d = 0; d < 7; ++d) {
                    int v[4] = {a, b, c, d};
                    bool ok = false;
                    for (int i = 0; i < 4 && !ok; ++i)
                        for (int j = i + 1; j < 4 && !ok; ++j) {
                            int m = v[i] > v[j] ? v[i] : v[j];
                            int min_other = 7;
                            for (int k2 = 0; k2 < 4; ++k2)
                                if (k2 != i && k2 != j && v[k2] < min_other)
                                    min_other = v[k2];
                            if (min_other == m) ok = true;
                        }
                    valid[a][b][c][d] = ok;
                }
}

/* DP: counts of sequences ending with each 3‑tuple after a certain length. */
seq_t a(idx_t n)
{
    /* n = 1 corresponds to length 4 arrays.  We start from length 3
       (unconstrained suffix), then add n elements to reach length n+3. */
    build_tables();

    static seq_t dp[343];
    static seq_t newdp[343];

    /* initialize counts for sequences of length 3: one for each suffix */
    for (int i = 0; i < 343; ++i) dp[i] = 1;

    /* iterate n times to add the remaining elements */
    for (idx_t step = 0; step < n; ++step) {
        memset(newdp, 0, sizeof(newdp));

        for (int s = 0; s < 343; ++s) {
            seq_t cur = dp[s];
            if (cur == 0) continue;          /* no sequences for this suffix */
            int a0 = triple_arr[s][0];
            int a1 = triple_arr[s][1];
            int a2 = triple_arr[s][2];

            for (int w = 0; w < 7; ++w)
                if (valid[a0][a1][a2][w]) {
                    int t = idx_map[a1][a2][w];
                    newdp[t] = newdp[t] + cur;
                }
        }
        memcpy(dp, newdp, sizeof(dp));
    }

    /* the answer is the total number of sequences of length n+3,
       i.e., the sum over all ending 3‑tuples. */
    seq_t total = 0;
    for (int i = 0; i < 343; ++i) total = total + dp[i];
    return total;
}
