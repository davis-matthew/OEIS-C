/*
 * A394829  Numbers of the form h*(9*h+1), where h = 0, -1, 1, -2, 2, -3, 3,
 *          ...
 * https://oeis.org/A394829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(9*n+2)/4 for n even; a(n) = (n+1)*(9*n+7)/4 for n odd
 * Range:     u8 n <= 4, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1) == 0) {                 /* n even */
        seq_t num = nn * (((seq_t)9 * nn) + (seq_t)2);
        return num / (seq_t)4;
    } else {                             /* n odd */
        seq_t num = (nn + (seq_t)1) * (((seq_t)9 * nn) + (seq_t)7);
        return num / (seq_t)4;
    }
}
