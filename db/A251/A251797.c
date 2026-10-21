/*
 * A251797  Number of (n+1)X(4+1) 0..3 arrays with no 2X2 subblock having
 *          x11-x00 less than x10-x01.
 * https://oeis.org/A251797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of (n+1)×5 arrays over {0,1,2,3} in which for every 2×2
 *            subblock ((x00,x01),(x10,x11)) we have x11 - x00 >= x10 - x01.
 *            Computation is performed by dynamic programming over row states
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define WIDTH        5
#define STATES       1024           /* 4^5 possible rows */
#define MAX_N        10000           /* maximum n expected by the tests */

static int  digits[STATES][WIDTH];
static int  adj[STATES][STATES];
static unsigned short deg[STATES];

static seq_t dp[STATES];
static seq_t dp_next[STATES];
static seq_t memo[MAX_N+1];
static size_t max_computed = 0;
static bool   initialized = false;

/* Build the digit tables and the adjacency list once. */
static void prepare(void)
{
    if (initialized) return;
    initialized = true;

    /* digit tables: for state s (0..1023) store its 5 base‑4 digits
       (digits[s][0] is the leftmost column, digits[s][4] the rightmost). */
    for (int s = 0; s < STATES; ++s) {
        int x = s;
        for (int pos = WIDTH-1; pos >= 0; --pos) {
            digits[s][pos] = x % 4;
            x              /= 4;
        }
    }

    /* adjacency: for every pair of states check the inequalities
       x11 - x00 >= x10 - x01  <=>  x11 + x01 >= x10 + x00. */
    for (int u = 0; u < STATES; ++u) {
        unsigned short d = 0;
        for (int v = 0; v < STATES; ++v) {
            bool ok = true;
            for (int j = 0; j < WIDTH-1; ++j) {
                if (digits[v][j+1] + digits[u][j+1] < digits[v][j] + digits[u][j]) {
                    ok = false;
                    break;
                }
            }
            if (ok) adj[u][d++] = v;
        }
        deg[u] = d;
    }

    /* initial dp: every state is possible for the first row */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)1;
    memo[0] = (seq_t)0;          /* unused, keeps indices aligned */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* below-offset behaviour irrelevant */

    prepare();

    if (n > max_computed) {
        for (size_t target = max_computed + 1; target <= n; ++target) {
            /* reset dp_next */
            for (int i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

            /* transition from dp to dp_next using adjacency */
            for (int u = 0; u < STATES; ++u) {
                seq_t val = dp[u];
                if (val == (seq_t)0) continue;
                for (unsigned short k = 0; k < deg[u]; ++k) {
                    int v = adj[u][k];
                    dp_next[v] += val;
                }
            }

            /* sum of dp_next gives the total number of arrays of height target+1 */
            seq_t total = (seq_t)0;
            for (int i = 0; i < STATES; ++i) total += dp_next[i];
            memo[target] = total;

            /* prepare dp for the next iteration: dp <- dp_next */
            for (int i = 0; i < STATES; ++i) dp[i] = dp_next[i];
        }
        max_computed = n;
    }
    return memo[n];
}
