/*
 * A255687  a(n) = n*(n + 1)*(7*n + 11)/6.
 * https://oeis.org/A255687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n + 11)/6
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n1 = n0 + 1;
    seq_t c  = 7 * n0 + 11;

    /* divide by 2 first; at least one of n0, n1, c is even */
    if ((n0 & 1) == 0) {
        n0 /= 2;
    } else if ((n1 & 1) == 0) {
        n1 /= 2;
    } else {
        c /= 2;
    }

    /* divide by 3 next; at least one of n0, n1, c is divisible by 3 */
    if ((n0 % 3) == 0) {
        n0 /= 3;
    } else if ((n1 % 3) == 0) {
        n1 /= 3;
    } else {
        c /= 3;
    }

    /* final multiplication */
    return n0 * n1 * c;
}
