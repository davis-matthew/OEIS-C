/*
 * A360268  A version of the Josephus problem: a(n) is the surviving integer
 *          under the following elimination process. Arrange 1,2,3,...,n in a
 *          circle, increasing clockwise. Starting with i=1, delete the
 *          integer 5 places clockwise from i. Repeat, counting 5 places from
 *          the next undeleted integer, until only o...
 * https://oeis.org/A360268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = (a(n-1) + 5) mod n + 1 for n > 1
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        res = ((res + (seq_t)5) % (seq_t)i) + (seq_t)1;
    }
    return res;
}
