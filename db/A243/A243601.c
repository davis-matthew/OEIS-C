/*
 * A243601  Number of length n+2 0..4 arrays with no three elements in a row
 *          with pattern aba (with a!=b) and new values 0..4 introduced in
 *          0..4 order.
 * https://oeis.org/A243601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Derived from the combinatorial definition of A243601:
    Count arrays of length n+2 over alphabet {0,1,2,3,4}, new values introduced
    in order (0,1,2,3,4), avoiding the pattern a b a with a≠b.  The DP
    state consists of the last two symbols (prev2, prev1) and the current
    maximum value used (m).  States are indexed by 0–4 for real symbols and
    5 for “undefined” (initially none).  The recurrence follows directly
    from the transition rules described in the OEIS entry. */

#include <stddef.h>

#define NONE 5               /* index representing “no symbol yet” */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset starts at 1; behaviour below is irrelevant */
        return 0;

    const idx_t L = n + 2;   /* length of the arrays to count */

    /* DP arrays: dp_cur[prev2][prev1][m] */
    seq_t dp_cur[6][6][6];
    seq_t dp_next[6][6][6];

    /* initialise all counts to 0 */
    for (int p2 = 0; p2 <= 5; ++p2)
        for (int p1 = 0; p1 <= 5; ++p1)
            for (int m = 0; m <= 5; ++m)
                dp_cur[p2][p1][m] = 0;

    /* initial state: no symbols placed yet, max value 0 */
    dp_cur[NONE][NONE][0] = 1;

    /* transition over each position of the length‑L array */
    for (idx_t pos = 0; pos < L; ++pos) {
        /* zero the next DP layer */
        for (int p2 = 0; p2 <= 5; ++p2)
            for (int p1 = 0; p1 <= 5; ++p1)
                for (int m = 0; m <= 5; ++m)
                    dp_next[p2][p1][m] = 0;

        /* iterate over all current states */
        for (int p2 = 0; p2 <= 5; ++p2) {
            for (int p1 = 0; p1 <= 5; ++p1) {
                for (int m = 0; m <= 5; ++m) {
                    const seq_t cur = dp_cur[p2][p1][m];
                    if (cur == 0)
                        continue;

                    /* try all possible next symbols c (0..m) */
                    for (int c = 0; c <= m; ++c) {
                        int new_m = m + (c == m ? 1 : 0);
                        if (new_m > 5)
                            continue;                /* cannot introduce >5 distinct values */

                        /* check forbidden pattern a b a */
                        if (p2 != NONE && p2 == c && p1 != c)
                            continue;

                        int new_p2 = p1;
                        int new_p1 = c;

                        dp_next[new_p2][new_p1][new_m] = dp_next[new_p2][new_p1][new_m] + cur;
                    }
                }
            }
        }

        /* copy next layer into current for next iteration */
        for (int p2 = 0; p2 <= 5; ++p2)
            for (int p1 = 0; p1 <= 5; ++p1)
                for (int m = 0; m <= 5; ++m)
                    dp_cur[p2][p1][m] = dp_next[p2][p1][m];
    }

    /* sum all valid end states */
    seq_t total = 0;
    for (int p2 = 0; p2 <= 5; ++p2)
        for (int p1 = 0; p1 <= 5; ++p1)
            for (int m = 0; m <= 5; ++m)
                total = total + dp_cur[p2][p1][m];

    return total;
}
