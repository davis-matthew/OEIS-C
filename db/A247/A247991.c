/*
 * A247991  Number of length n+5 0..4 arrays with no disjoint triples in any
 *          consecutive six terms having the same sum.
 * https://oeis.org/A247991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 0..4 arrays with no disjoint triples
 *            in any consecutive six terms having the same sum. This is a DP
 *            over the last five digits; see implementation for details
 * Range:     u8 none, u16 n <= 2, u32 n <= 12, u64 n <= 30, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Transition table: for each 5‑digit state s (encoded in base 5)
       and each possible next digit x (0..4), store the index of
       the new state if the 6‑digit window s followed by x is valid,
       otherwise -1.  Build it once. */
    static int nextState[3125][5];
    static int built = 0;
    if (!built)
    {
        for (int s = 0; s < 3125; ++s) {
            int d[5];
            int t = s;
            for (int i = 0; i < 5; ++i) {
                d[i] = t % 5;
                t /= 5;
            }
            for (int x = 0; x < 5; ++x) {
                int a6[6] = { d[0], d[1], d[2], d[3], d[4], x };
                int sum6 = 0;
                for (int i = 0; i < 6; ++i) sum6 += a6[i];
                int ok = 1;
                for (int i = 0; i < 4 && ok; ++i)
                    for (int j = i + 1; j < 5 && ok; ++j)
                        for (int k = j + 1; k < 6 && ok; ++k) {
                            int tri = a6[i] + a6[j] + a6[k];
                            if (sum6 == 2 * tri) ok = 0;
                        }
                if (ok) {
                    int ns = d[1] + 5 * d[2] + 25 * d[3] + 125 * d[4] + 625 * x;
                    nextState[s][x] = ns;
                } else
                    nextState[s][x] = -1;
            }
        }
        built = 1;
    }

    /* DP arrays: cur holds counts for sequences ending with state s
       after the current prefix length.  nxt is a temporary array. */
    static seq_t cur[3125], nxt[3125];

    /* Initialize DP for length 5 (any 5‑tuple is allowed). */
    for (int s = 0; s < 3125; ++s)
        cur[s] = (seq_t)1;

    idx_t steps = n;                 /* number of new digits to add */
    for (idx_t step = 0; step < steps; ++step) {
        /* zero nxt */
        for (int s = 0; s < 3125; ++s) nxt[s] = 0;
        /* transition */
        for (int s = 0; s < 3125; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            for (int x = 0; x < 5; ++x) {
                int ns = nextState[s][x];
                if (ns >= 0) nxt[ns] += val;
            }
        }
        /* copy back */
        for (int s = 0; s < 3125; ++s) cur[s] = nxt[s];
    }

    /* sum all valid sequences of length N = n+5 */
    seq_t total = 0;
    for (int s = 0; s < 3125; ++s)
        total += cur[s];
    return total;
}
