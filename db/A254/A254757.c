/*
 * A254757  Part of the positive proper solutions x of the Pell equation x^2 -
 *          2*y^2 = - 7^2 based on the fundamental solution (x0, y0)= (-1, 5).
 * https://oeis.org/A254757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) with a(1)=17, a(2)=103
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)103;

    seq_t prevprev = (seq_t)17; /* a(1) */
    seq_t prev     = (seq_t)103;/* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)6 * prev - prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
