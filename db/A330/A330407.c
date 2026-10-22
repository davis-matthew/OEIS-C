/*
 * A330407  Number of ordered integer pairs (b,c) with -n <= b <= n and -n <=
 *          c <= n such that both roots of x^2 + b*x + c = 0 are distinct
 *          integers.
 * https://oeis.org/A330407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

   OEIS A330407
   Number of ordered integer pairs (b,c) with -n ≤ b ≤ n and -n ≤ c ≤ n
   such that both roots of x^2 + b*x + c = 0 are distinct integers.

   The sequence can be computed by counting unordered pairs of distinct
   integer roots (r1,r2).  For a pair (r1,r2) we have

        b = -(r1 + r2),   c = r1*r2 .

   The pair contributes a valid (b,c) iff

        |r1 + r2| ≤ n  and  |r1*r2| ≤ n .

   For fixed r1 we can find all admissible r2 in a single interval, so
   the whole counting is O(n).

*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* local cast – n fits into int as tests are ≤ 10 000 */
    int N = (int)n;
    seq_t total = 0;

    for (int r1 = -N; r1 <= N; ++r1) {
        if (r1 == 0) {
            /* product constraint is always satisfied;
               r2 must satisfy 1 ≤ r2 ≤ N and |r2| ≤ N */
            int lower = 1;
            int upper = N;
            if (lower <= upper)
                total += (seq_t)(upper - lower + 1);
            continue;
        }

        int absR = r1 > 0 ? r1 : -r1;   /* |r1|   */
        int bound = N / absR;           /* floor( N / |r1| ) */

        int sum_low  = -N - r1;         /* lower bound from |r1+r2| ≤ N   */
        int sum_high =  N - r1;         /* upper bound from |r1+r2| ≤ N   */
        int prod_low = -bound;          /* lower bound from |r1*r2| ≤ N   */
        int prod_high = bound;          /* upper bound from |r1*r2| ≤ N   */

        int lower = r1 + 1;             /* r2 must be > r1 */
        if (sum_low  > lower) lower = sum_low;
        if (prod_low > lower) lower = prod_low;

        int upper = sum_high;
        if (prod_high < upper) upper = prod_high;

        if (lower <= upper)
            total += (seq_t)(upper - lower + 1);
    }

    return total;
}
