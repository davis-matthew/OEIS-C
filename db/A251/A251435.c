/*
 * A251435  Number of (n+1)X(1+1) 0..3 arrays with no 2X2 subblock having the
 *          minimum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of (n+1)×2 arrays over {0,1,2,3} whose
 *            every 2×2 subblock satisfies min(diagonal elements) ≤
 *            |difference of antidiagonal elements|
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The transition between consecutive rows is determined by this rule. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) /* out of OEIS offset */
        return 0;

    /* Transition table: T[i] holds a bitmask of all rows j that can follow row i. */
    static unsigned short T[16];
    static bool ready = false;
    if (!ready) {                /* Build the table once */
        for (int i = 0; i < 16; ++i) {
            int a = i / 4;         /* first element of the row i */
            int b = i % 4;         /* second element of the row i */
            unsigned short mask = 0;
            for (int j = 0; j < 16; ++j) {
                int c = j / 4;     /* first element of the row j */
                int d = j % 4;     /* second element of the row j */
                int min_ad = (a < d) ? a : d;           /* min(diagonal)  */
                int diff_bc = (b > c) ? b - c : c - b;   /* |b-c|         */
                if (min_ad <= diff_bc)                  /* allowed transition */
                    mask |= 1u << j;
            }
            T[i] = mask;
        }
        ready = true;
    }

    /* Dynamic programming over rows */
    seq_t dp[16], next[16];
    for (int i = 0; i < 16; ++i)
        dp[i] = 1;          /* any first row is admissible */

    for (idx_t step = 1; step <= n; ++step) { /* n steps → n+1 rows */
        for (int j = 0; j < 16; ++j)
            next[j] = 0;

        for (int i = 0; i < 16; ++i) {
            unsigned short m = T[i];
            /* iterate over set bits of the mask */
            while (m) {
                int j = __builtin_ctz(m);
                next[j] += dp[i];
                m &= m - 1;
            }
        }

        for (int j = 0; j < 16; ++j)
            dp[j] = next[j];
    }

    /* Sum over all ending rows to obtain a(n) */
    seq_t sum = 0;
    for (int j = 0; j < 16; ++j)
        sum += dp[j];
    return sum;
}
