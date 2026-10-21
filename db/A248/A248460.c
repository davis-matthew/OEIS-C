/*
 * A248460  Number of length n+2 0..7 arrays with no three consecutive terms
 *          having the sum of any two elements equal to twice the third.
 * https://oeis.org/A248460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of length n+2 arrays of digits 0..7 with no three
 *            consecutive terms containing an element that is the arithmetic
 *            mean of the other two. (i.e., for all consecutive triples
 *            (x,y,z), none of x+y==2*z, x+z==2*y, or y+z==2*x hold.)
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Defined only for n >= 1; return 0 otherwise. */
    if (n < 1) return 0;

    /* Length of the array to count: L = n + 2. */
    idx_t L = n + 2;          /* L >= 3 since n >= 1 */

    /* There are 8^2 = 64 possible pairs of last two digits. */
    const int STATES = 64;
    seq_t dp[STATES];
    seq_t next[STATES];

    /* Initial sequences of length 2: every pair occurs exactly once. */
    for (int s = 0; s < STATES; ++s) dp[s] = (seq_t)1;

    /* Extend the sequences to length L. */
    for (idx_t len = 3; len <= L; ++len) {
        /* Clear the next array. */
        for (int i = 0; i < STATES; ++i) next[i] = (seq_t)0;

        /* For each existing suffix pair (x,y). */
        for (int s = 0; s < STATES; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;      /* skip empty states */

            int x = s >> 3;          /* high 3 bits: previous-previous digit */
            int y = s & 7;           /* low 3 bits: previous digit */

            /* Try every possible new digit z. */
            for (int z = 0; z < 8; ++z) {
                /* Reject if any of the three mean conditions holds. */
                if (x + y == 2 * z) continue;
                if (x + z == 2 * y) continue;
                if (y + z == 2 * x) continue;

                int ns = (y << 3) | z;    /* new suffix pair (y,z) */
                next[ns] += cur;
            }
        }

        /* Move to the next length. */
        for (int i = 0; i < STATES; ++i) dp[i] = next[i];
    }

    /* Sum over all ending states to get the total number of arrays. */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATES; ++s) total += dp[s];
    return total;
}
