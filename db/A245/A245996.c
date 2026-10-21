/*
 * A245996  Number of length 1+2 0..n arrays with no pair in any consecutive
 *          three terms totaling exactly n.
 * https://oeis.org/A245996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n mod 2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn * nn;
    if (n & 1) res = res + 1;
    return res;
}
