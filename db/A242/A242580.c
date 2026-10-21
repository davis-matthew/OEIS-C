/*
 * A242580  Number of length n+3 0..4 arrays with no four elements in a row
 *          with pattern aabb (with a!=b) and new values 0..4 introduced in
 *          0..4 order.
 * https://oeis.org/A242580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Compute A242580: number of length (n+3) sequences over {0,1,2,3,4}
 * with the property that new symbols appear in increasing order
 * (restricted growth function) and no four consecutive elements form
 * the pattern aabb with a != b.
 * The computation is done by dynamic programming over positions,
 * keeping the current maximum symbol and the last three symbols.
 */
#include <stddef.h>

#define S_MAX 4          /* maximum symbol value (0..4) */
#define S_SENT 5         /* sentinel value for “no symbol yet” */

seq_t a(idx_t n) {
    /* Number of symbols to generate */
    idx_t N = n + 3;           /* length of the sequence */

    /* DP tables: [max][s0][s1][s2] where s0,s1,s2 ∈ 0..4 or sentinel 5 */
    seq_t cur[S_MAX+1][S_SENT+1][S_SENT+1][S_SENT+1];
    seq_t nxt[S_MAX+1][S_SENT+1][S_SENT+1][S_SENT+1];

    /* Initialize all entries to zero */
    for (int m = 0; m <= S_MAX; ++m)
        for (int a0 = 0; a0 <= S_SENT; ++a0)
            for (int a1 = 0; a1 <= S_SENT; ++a1)
                for (int a2 = 0; a2 <= S_SENT; ++a2)
                    cur[m][a0][a1][a2] = 0;

    /* Start with the first element fixed to 0 */
    cur[0][S_SENT][S_SENT][0] = 1;

    /* Iterate over remaining positions (from 2 to N) */
    for (idx_t step = 2; step <= N; ++step) {
        /* Zero out next table */
        for (int m = 0; m <= S_MAX; ++m)
            for (int a0 = 0; a0 <= S_SENT; ++a0)
                for (int a1 = 0; a1 <= S_SENT; ++a1)
                    for (int a2 = 0; a2 <= S_SENT; ++a2)
                        nxt[m][a0][a1][a2] = 0;

        /* Transition from current states to next states */
        for (int max = 0; max <= S_MAX; ++max) {
            for (int s0 = 0; s0 <= S_SENT; ++s0) {
                for (int s1 = 0; s1 <= S_SENT; ++s1) {
                    for (int s2 = 0; s2 <= S_SENT; ++s2) {

                        seq_t cnt = cur[max][s0][s1][s2];
                        if (cnt == 0) continue;

                        /* Possible next symbols: 0 .. max+1 (max+1 ≤ 5) */
                        for (int k = 0; k <= max + 1; ++k) {
                            if (k > S_MAX) continue;  /* cannot exceed 4 */

                            int newmax = (k == max + 1) ? k : max;

                            /* Avoid the forbidden pattern aabb */
                            if (s0 != S_SENT && s1 != S_SENT && s2 != S_SENT) {
                                if (s0 == s1 && s2 == k && s0 != s2)
                                    continue;  /* pattern detected */
                            }

                            /* Shift last three symbols */
                            int ns0 = s1;
                            int ns1 = s2;
                            int ns2 = k;

                            nxt[newmax][ns0][ns1][ns2] += cnt;
                        }
                    }
                }
            }
        }

        /* Swap current and next tables for the next iteration */
        for (int m = 0; m <= S_MAX; ++m)
            for (int a0 = 0; a0 <= S_SENT; ++a0)
                for (int a1 = 0; a1 <= S_SENT; ++a1)
                    for (int a2 = 0; a2 <= S_SENT; ++a2)
                        cur[m][a0][a1][a2] = nxt[m][a0][a1][a2];
    }

    /* Sum all counts for final position to get the answer */
    seq_t answer = 0;
    for (int max = 0; max <= S_MAX; ++max)
        for (int s0 = 0; s0 <= S_SENT; ++s0)
            for (int s1 = 0; s1 <= S_SENT; ++s1)
                for (int s2 = 0; s2 <= S_SENT; ++s2)
                    answer += cur[max][s0][s1][s2];

    return answer;
}
