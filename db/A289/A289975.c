/*
 * A289975  p-INVERT of the Fibonacci numbers (A000045, including 0), where
 *          p(S) = 1 - S - S^2.
 * https://oeis.org/A289975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) - a(n-4) * initial values:
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    /* a0 .. a3 correspond to a(n-4), a(n-3), a(n-2), a(n-1) respectively */
    seq_t a0 = (seq_t)0;   /* a(0)   */
    seq_t a1 = (seq_t)1;   /* a(1)   */
    seq_t a2 = (seq_t)1;   /* a(2)   */
    seq_t a3 = (seq_t)4;   /* a(3)   */
    seq_t an = a3;         /* initialise to valid value, will be overwritten */

    for (idx_t i = 4; i <= n; ++i) {
        an = 2*a3 + 2*a2 - 3*a1 - a0;
        a0 = a1;   /* shift window: a0 <- a1 */
        a1 = a2;   /* a1 <- a2 */
        a2 = a3;   /* a2 <- a3 */
        a3 = an;   /* a3 <- new term */
    }

    return an;
}
