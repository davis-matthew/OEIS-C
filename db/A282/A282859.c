/*
 * A282859  Number of nX5 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, vertical and antidiagonal neighbors.
 * https://oeis.org/A282859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of n × 5 binary arrays where each 1 has at most one neighbor
 * among its horizontal, vertical and antidiagonal neighbors.
 * The algorithm uses a row‑by‑row DP on a 3^5‑state automaton.
 */

#include <stdint.h>

static const int POW3[5] = {1, 3, 9, 27, 81};

/* transition data structures ------------------------------------------------
 * transCount[s]  : number of different next states reachable from state s
 * transNext[s][k]: the k‑th next state from state s
 * transLen[s]    : number of transitions from state s
 */
static int transLen[243];
static uint8_t transNext[243][32];
static uint8_t transCount[243][32];

/* fetch state digit in base‑3 representation */
static inline int get_digit(int state, int pos)
{
    return (state / POW3[pos]) % 3;
}

/* encode array of digits (0,1,2) to base‑3 state */
static inline int encode_state(const int d[5])
{
    int s = 0;
    s += d[0] * POW3[0];
    s += d[1] * POW3[1];
    s += d[2] * POW3[2];
    s += d[3] * POW3[3];
    s += d[4] * POW3[4];
    return s;
}

/* precompute the transition tables once ----------------------------------- */
static void init_transitions(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    for (int prev = 0; prev < 243; ++prev) {
        int prevDeg[5];
        for (int j = 0; j < 5; ++j) prevDeg[j] = get_digit(prev, j);

        unsigned char counter[243] = {0};

        for (int mask = 0; mask < 32; ++mask) {
            int degCur[5] = {0};
            int ok = 1;

            /* vertical and diagonal adjacency with previous row */
            for (int j = 0; j < 5 && ok; ++j) {
                if (mask & (1 << j)) {
                    /* vertical with column j */
                    if (prevDeg[j] == 0) {
                        /* none */
                    } else if (prevDeg[j] == 1) {
                        degCur[j] += 1;
                    } else {                    /* prevDeg[j] == 2 */
                        ok = 0; break;
                    }

                    /* diagonal from (prev, j+1) to current j */
                    if (j + 1 < 5) {
                        if (prevDeg[j + 1] == 0) {}
                        else if (prevDeg[j + 1] == 1) {
                            degCur[j] += 1;
                        } else {    /* prevDeg[j+1] == 2 */
                            ok = 0; break;
                        }
                    }

                    if (degCur[j] >= 2) { ok = 0; break; }
                }
            }
            if (!ok) continue;

            /* horizontal adjacency inside current row */
            for (int j = 0; j < 4 && ok; ++j) {
                if ((mask & (1 << j)) && (mask & (1 << (j + 1)))) {
                    if (degCur[j] >= 1 || degCur[j + 1] >= 1) {
                        ok = 0; break;
                    }
                    degCur[j] += 1;
                    degCur[j + 1] += 1;
                    if (degCur[j] >= 2 || degCur[j + 1] >= 2) {
                        ok = 0; break;
                    }
                }
            }
            if (!ok) continue;

            /* encode next state */
            int nxt = 0;
            for (int j = 0; j < 5; ++j) {
                if (mask & (1 << j)) {
                    if (degCur[j] == 0)      nxt += 1 * POW3[j];
                    else /* degCur[j] == 1 */ nxt += 2 * POW3[j];
                }
            }
            counter[nxt] += 1;
        }

        /* store transitions */
        int len = 0;
        for (int nxt = 0; nxt < 243; ++nxt) {
            if (counter[nxt]) {
                transNext[prev][len] = (uint8_t)nxt;
                transCount[prev][len] = counter[nxt];
                ++len;
            }
        }
        transLen[prev] = len;
    }
}

/* ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    init_transitions();

    seq_t dp[243] = {0};
    seq_t ndp[243];
    dp[0] = 1;               /* initial empty previous row */

    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < 243; ++i) ndp[i] = 0;

        for (int s = 0; s < 243; ++s) {
            if (dp[s] == 0) continue;
            int len = transLen[s];
            for (int k = 0; k < len; ++k) {
                int t  = transNext[s][k];
                int cnt = transCount[s][k];
                for (int c = 0; c < cnt; ++c) ndp[t] += dp[s];
            }
        }
        for (int i = 0; i < 243; ++i) dp[i] = ndp[i];
    }

    seq_t res = 0;
    for (int i = 0; i < 243; ++i) res += dp[i];
    return res;
}
