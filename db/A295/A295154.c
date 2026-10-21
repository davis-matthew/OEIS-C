/*
 * A295154  Numbers that have exactly six representations as a sum of five
 *          nonnegative squares.
 * https://oeis.org/A295154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: enumerate unordered representations of n as a sum of five nonnegative squares;
   the nth term a(n) of OEIS A295154 is the nth integer having exactly 6 such representations.
   Since the sequence is finite, we precompute all such integers up to the proven bound 5408. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_N 5408
#define MAX_USED 5
#define MAX_SQUARES 74  /* squares of 0..73 */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static bool initialized = false;
    static seq_t terms[MAX_N + 1];
    static idx_t term_count = 0;

    if (!initialized) {
        /* arrays for DP: cur[used][sum], nxt[used][sum] */
        static seq_t cur[MAX_USED + 1][MAX_N + 1];
        static seq_t nxt[MAX_USED + 1][MAX_N + 1];
        int squares[MAX_SQUARES];
        int sqc = 0;

        /* compute square values 0^2 .. 73^2 */
        for (int i = 0; i <= 73; ++i) {
            squares[sqc++] = i * i;
        }

        /* initialise DP */
        for (int u = 0; u <= MAX_USED; ++u)
            for (idx_t s = 0; s <= MAX_N; ++s)
                cur[u][s] = 0;
        cur[0][0] = 1;

        /* process each square value */
        for (int si = 0; si < sqc; ++si) {
            int sq = squares[si];
            /* clear nxt */
            for (int u = 0; u <= MAX_USED; ++u)
                for (idx_t s = 0; s <= MAX_N; ++s)
                    nxt[u][s] = 0;
            /* DP transition */
            for (int u = 0; u <= MAX_USED; ++u) {
                for (idx_t s = 0; s <= MAX_N; ++s) {
                    seq_t cur_val = cur[u][s];
                    if (cur_val == 0) continue;
                    int maxm = MAX_USED - u;
                    for (int m = 0; m <= maxm; ++m) {
                        int new_u = u + m;
                        idx_t new_s = s + (idx_t)m * (idx_t)sq;
                        if (new_s > MAX_N) break;
                        nxt[new_u][new_s] += cur_val;
                    }
                }
            }
            /* copy nxt to cur */
            for (int u = 0; u <= MAX_USED; ++u)
                for (idx_t s = 0; s <= MAX_N; ++s)
                    cur[u][s] = nxt[u][s];
        }

        /* collect numbers with exactly 6 unordered representations */
        for (idx_t val = 1; val <= MAX_N; ++val) {
            if (cur[MAX_USED][val] == 6) {
                terms[term_count++] = (seq_t)val;
            }
        }

        initialized = true;
    }

    /* guard against out-of-range index (undefined behaviour otherwise) */
    if (n == 0 || (n - 1) >= term_count) {
        return 0;  /* sequence is finite; caller should provide valid n */
    }
    return terms[n - 1];
}
