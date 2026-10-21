/*
 * A249288  Number of length n+3 0..6 arrays with no four consecutive terms
 *          having the sum of any three elements equal to three times the
 *          fourth.
 * https://oeis.org/A249288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of length n+3 arrays over 0..6 with no four consecutive terms having
   the sum of any three elements equal to three times the fourth.
   Computed using a 3‑state DFA with 343 states.  */

#include <stdbool.h>

#define STATES 343

static int nexts[STATES][7];
static int next_count[STATES];
static bool graph_initialized = false;

static void init_graph(void)
{
    if (graph_initialized) return;
    for (int s = 0; s < STATES; ++s) {
        int a = s / 49;
        int b = (s / 7) % 7;
        int c = s % 7;
        int cnt = 0;
        for (int d = 0; d <= 6; ++d) {
            bool ok = true;
            if (a + b + c == 3 * d) ok = false;
            else if (a + b + d == 3 * c) ok = false;
            else if (a + c + d == 3 * b) ok = false;
            else if (b + c + d == 3 * a) ok = false;
            if (ok) {
                int ns = b * 49 + c * 7 + d;
                nexts[s][cnt++] = ns;
            }
        }
        next_count[s] = cnt;
    }
    graph_initialized = true;
}

seq_t a(idx_t n)
{
    init_graph();

    static seq_t dp[2][STATES];
    seq_t *cur = dp[0];
    seq_t *next = dp[1];

    for (int i = 0; i < STATES; ++i) cur[i] = (seq_t)1;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) next[i] = (seq_t)0;
        for (int s = 0; s < STATES; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0) continue;
            int cnt = next_count[s];
            for (int k = 0; k < cnt; ++k) {
                int ns = nexts[s][k];
                next[ns] += val;
            }
        }
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i) total += cur[i];
    return total;
}
