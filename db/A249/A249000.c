/*
 * A249000  Number of length n+4 0..7 arrays with no five consecutive terms
 *          having two times the sum of any three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A249000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 7, u64 n <= 18, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249000:  a(n) = number of length n+4 0..7 arrays with no five consecutive
   terms for which 2*(sum of any three) = 3*(sum of the remaining two) */

/* The program builds the set of forbidden 5‑tuples (one of the ten 3‑subset
   sums satisfies 5*s = 3*total) and then counts all length‑(n+4) strings
   over {0,…,7} that avoid any forbidden 5‑tuple.  The counting is done
   with a simple DP that keeps the last four digits as a state (4×3 bits = 12
   bits => 4096 states).  The algorithm is O((n+4)·8·4096), which easily
   meets the 2‑second limit for n≈10 000.  The function is memoisable between
   calls but recomputes each time; the DP arrays are re‑initialised for each
   call. */

#include <stdbool.h>

static const int combo[10][3] = {
    {0,1,2},{0,1,3},{0,1,4},{0,2,3},{0,2,4},
    {0,3,4},{1,2,3},{1,2,4},{1,3,4},{2,3,4}
};

static unsigned char forbidden[1<<15];
static bool forbidden_inited = false;

/* initialise forbidden[tuple] once */
static void init_forbidden(void)
{
    if (forbidden_inited) return;
    for (int a = 0; a < 8; ++a)
        for (int b = 0; b < 8; ++b)
            for (int c = 0; c < 8; ++c)
                for (int d = 0; d < 8; ++d)
                    for (int e = 0; e < 8; ++e) {
                        int id = (((((a<<3)|b)<<3|c)<<3|d)<<3)|e;
                        int digits[5] = {a,b,c,d,e};
                        int total = a+b+c+d+e;
                        bool bad = false;
                        for (int k = 0; k < 10 && !bad; ++k) {
                            int s = digits[combo[k][0]]
                                  + digits[combo[k][1]]
                                  + digits[combo[k][2]];
                            if (5*s == 3*total) bad = true;
                        }
                        forbidden[id] = bad;
                    }
    forbidden_inited = true;
}

seq_t a(idx_t n)
{
    init_forbidden();

    /* length of arrays */
    idx_t L = n + 4;
    idx_t steps = L - 4;            /* number of transitions to perform */
    /* DP arrays: dp[cur][state], dp[nxt][state] */
    static seq_t dp[2][4096];
    int cur = 0, nxt = 1;

    /* initialise with 1 for all 4‑digit prefixes */
    for (int i = 0; i < 4096; ++i) dp[cur][i] = 1;

    /* perform transitions */
    for (idx_t step = 0; step < steps; ++step) {
        /* clear next array */
        for (int i = 0; i < 4096; ++i) dp[nxt][i] = 0;

        for (int state = 0; state < 4096; ++state) {
            seq_t val = dp[cur][state];
            if (val == 0) continue;          /* fast path */
            for (int d = 0; d < 8; ++d) {
                int tuple_id = (state << 3) | d;
                if (!forbidden[tuple_id]) {
                    int new_state = tuple_id & 4095;
                    dp[nxt][new_state] += val;
                }
            }
        }
        /* swap current and next */
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum over all states */
    seq_t total = 0;
    for (int i = 0; i < 4096; ++i) total += dp[cur][i];
    return total;
}
