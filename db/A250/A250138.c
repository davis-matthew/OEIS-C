/*
 * A250138  Number of length n+2 0..6 arrays with the medians of every three
 *          consecutive terms nondecreasing.
 * https://oeis.org/A250138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 28, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) counts length n+2 arrays of values 0..6 whose medians of every
 * 3 consecutive terms are nondecreasing.
 * The algorithm uses dynamic programming on the last three terms.
 * A state is the tuple (a,b,c) with 0≤a,b,c≤6, encoded as a*49+b*7+c.
 * Transition from state (a,b,c) to (b,c,d) (0≤d≤6) is allowed iff
 * median(a,b,c) ≤ median(b,c,d).  For length 3 every state is valid,
 * giving 7³=343 sequences.
 */

#include <stdint.h>
#include <stddef.h>

static inline int median3(int a,int b,int c)
{
    int minv = a<b?a:b;
    minv = minv<c?minv:c;
    int maxv = a>b?a:b;
    maxv = maxv>c?maxv:c;
    return a+b+c - minv - maxv;
}

seq_t a(idx_t n)
{
    /* length of the array being counted */
    idx_t len = n + 2;
    if (len <= 3) {
        /* for length 3 the answer is 7^3 = 343 */
        return (seq_t)343;
    }

    /* state arrays: 7*7*7 = 343 states */
    seq_t dp[343];
    seq_t dp_next[343];

    /* base: all states have count 1 at length 3 */
    for (size_t i = 0; i < 343; ++i) {
        dp[i] = 1;
    }

    /* perform transitions for each additional element */
    size_t steps = (size_t)(len - 3);
    for (size_t s = 0; s < steps; ++s) {
        /* zero next state array */
        for (size_t i = 0; i < 343; ++i) {
            dp_next[i] = 0;
        }

        /* transition for each current state */
        for (size_t i = 0; i < 343; ++i) {
            seq_t cur = dp[i];
            if (cur == 0) continue;

            int a = i / 49;
            int bc = i % 49;
            int b = bc / 7;
            int c = bc % 7;
            int base_bc = b * 49 + c * 7;
            int m1 = median3(a, b, c);

            for (int d = 0; d < 7; ++d) {
                int idx_next = base_bc + d;
                int m2 = median3(b, c, d);
                if (m1 <= m2) {
                    dp_next[idx_next] += cur;
                }
            }
        }

        /* move to next state array */
        for (size_t i = 0; i < 343; ++i) {
            dp[i] = dp_next[i];
        }
    }

    /* sum counts over all states to obtain the final answer */
    seq_t result = 0;
    for (size_t i = 0; i < 343; ++i) {
        result += dp[i];
    }
    return result;
}
