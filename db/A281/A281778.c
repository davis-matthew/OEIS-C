/*
 * A281778  Number of distinct topologies on an n-set that have exactly 10
 *          open sets.
 * https://oeis.org/A281778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24 * S(n,4) + 660 * S(n,5) + 6570 * S(n,6) + 37800 *
 *            S(n,7) + 141120 * S(n,8) + 362880 * S(n,9) where S(n,k) is
 *            Stirling number of the second kind. This is computed iteratively
 *            using the recurrence: S(n,k) = k * S(n-1,k) + S(n-1,k-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* local arrays for current and next row of Stirling numbers */
    seq_t curr[10] = {0};
    seq_t next[10] = {0};

    /* base row S(0,0)=1; others zero */
    curr[0] = (seq_t)1;

    /* iterate from 1 to n to fill Stirling numbers */
    for (idx_t i = 1; i <= n; ++i) {
        /* next[0] is always 0 for n>0 */
        next[0] = (seq_t)0;
        for (int k = 1; k <= 9; ++k) {
            /* next[k] = k * curr[k] + curr[k-1] */
            seq_t term1 = (seq_t)k * curr[k];
            seq_t term2 = curr[k-1];
            next[k] = term1 + term2;
        }
        /* copy next row into curr for next iteration */
        for (int k = 0; k <= 9; ++k) {
            curr[k] = next[k];
        }
    }

    /* compute the final sum using the given coefficients */
    seq_t ans = (seq_t)0;
    ans += (seq_t)24      * curr[4];
    ans += (seq_t)660     * curr[5];
    ans += (seq_t)6570    * curr[6];
    ans += (seq_t)37800   * curr[7];
    ans += (seq_t)141120  * curr[8];
    ans += (seq_t)362880  * curr[9];

    return ans;
}
