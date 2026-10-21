/*
 * A271937  a(n) = (7/4)*n^2 + (5/2)*n + (7 + (-1)^n)/8.
 * https://oeis.org/A271937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7/4)*n^2 + (5/2)*n + (7 + (-1)^n)/8
 * Range:     u8 n <= 3, u16 n <= 168, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = 14 * nn * nn + 20 * nn + 7;
    if (n & 1ULL)
        num -= 1;   /* odd n: subtract 1 */
    else
        num += 1;   /* even n: add 1 */

    return num / 8;
}
