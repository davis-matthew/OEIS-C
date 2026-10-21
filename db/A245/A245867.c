/*
 * A245867  Number of length n+2 0..6 arrays with some pair in every
 *          consecutive three terms totalling exactly 6.
 * https://oeis.org/A245867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting sequence A245867:
   Number of length n+2 arrays over {0,…,6} such that in every
   consecutive triple there exists a pair summing to 6.
   Computation is done by dynamic programming over the last two
   elements.  The algorithm has O(n) time and constant memory. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: the sequence is defined for n ≥ 1.  For
       n == 0 the function returns 0. */
    if (n == 0)
        return (seq_t)0;

    /* Transition table: valid[s][c] is true iff
       the triple (a,b,c) with s = 7*a + b satisfies
       a+b == 6 or b+c == 6 or a+c == 6. */
    static bool valid[49][7];
    static bool initialized = false;
    if (!initialized) {
        for (int a = 0; a <= 6; ++a) {
            for (int b = 0; b <= 6; ++b) {
                int s = 7 * a + b;
                for (int c = 0; c <= 6; ++c) {
                    bool ok = ((a + b) == 6) || ((b + c) == 6) || ((a + c) == 6);
                    valid[s][c] = ok;
                }
            }
        }
        initialized = true;
    }

    /* DP arrays: current counts of sequences ending with
       each ordered pair (a,b).  There are 7*7 = 49 states. */
    seq_t cur[49];
    seq_t nxt[49];

    /* Initial state: any pair (a,b) is allowed for length 2. */
    for (int i = 0; i < 49; ++i)
        cur[i] = (seq_t)1;

    /* Perform n transitions to reach length n+2. */
    for (idx_t step = 0; step < n; ++step) {
        /* Reset next array to zero. */
        for (int i = 0; i < 49; ++i)
            nxt[i] = (seq_t)0;

        /* Transition: for each state (a,b) add a new element c. */
        for (int s = 0; s < 49; ++s) {
            seq_t val = cur[s];
            if (val == (seq_t)0)
                continue;
            int a = s / 7;
            int b = s % 7;
            for (int c = 0; c <= 6; ++c) {
                if (valid[s][c]) {
                    int t = 7 * b + c;   /* new state (b,c) */
                    nxt[t] += val;
                }
            }
        }

        /* Swap cur and nxt for the next iteration. */
        for (int i = 0; i < 49; ++i) {
            cur[i] = nxt[i];
        }
    }

    /* Sum over all ending states to get the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 49; ++i)
        total += cur[i];

    return total;
}
