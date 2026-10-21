/*
 * A253724  Numbers c(n) whose squares are equal to the sums of a number M(n)
 *          of consecutive cubed integers b^3 + (b+1)^3 + ... + (b+M-1)^3 =
 *          c^2, starting at b(n) (A002593) for M(n) being twice a squared
 *          integer (A001105).
 * https://oeis.org/A253724
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 * (4*n^4 - 1)
 * Range:     u8 none, u16 n <= 4, u32 n <= 19, u64 n <= 463, u128 n <= 9967,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety we ignore n < 2 because the OEIS offset is 2.
       The formula still works for any n >= 1. */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */
    seq_t n7 = n4 * n3;          /* n^7 = n^4 * n^3 */
    seq_t res = (n7 << 2) - n3;  /* 4*n^7 - n^3 */
    return res;
}
