/*
 * A247531  Number of length n+3 0..6 arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 22, u64 n <= 52, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247531
   Number of length n+3 0..6 arrays with some disjoint pairs in every consecutive
   four terms having the same sum.
   Uses a dynamic programming over the last three values; each step
   checks the 4‑term window (a,b,c,x) satisfies a+b==c+x or a+c==b+x or a+x==b+c.
*/
#include <stdbool.h>

static const int STATE_SIZE = 343; /* 7^3 */

seq_t a(idx_t n) {
    /* Length of the array */
    idx_t L = n + 3;          /* since n starts at 1 */
    /* DP arrays */
    seq_t current[STATE_SIZE];
    seq_t next[STATE_SIZE];
    seq_t *dp_cur = current;
    seq_t *dp_next = next;
    /* Initialize: all 3‑tuples are valid before any 4‑tuple check */
    for (int i = 0; i < STATE_SIZE; ++i) {
        dp_cur[i] = (seq_t)1;
        dp_next[i] = (seq_t)0;
    }
    /* Iterate adding one element at a time */
    for (idx_t len = 3; len < L; ++len) {
        /* clear next DP state */
        for (int i = 0; i < STATE_SIZE; ++i) dp_next[i] = (seq_t)0;
        for (int idx = 0; idx < STATE_SIZE; ++idx) {
            seq_t cnt = dp_cur[idx];
            if (cnt == (seq_t)0) continue;
            /* decode the 3‑tuple */
            int a = idx / 49;
            int rem = idx % 49;
            int b = rem / 7;
            int c = rem % 7;
            for (int x = 0; x <= 6; ++x) {
                /* Check the 4‑tuple condition */
                if ( (a + b == c + x) ||
                     (a + c == b + x) ||
                     (a + x == b + c) ) {
                    int idx2 = b * 49 + c * 7 + x;
                    dp_next[idx2] += cnt;
                }
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }
    /* Sum all counts for full-length arrays */
    seq_t result = (seq_t)0;
    for (int i = 0; i < STATE_SIZE; ++i) result += dp_cur[i];
    return result;
}
