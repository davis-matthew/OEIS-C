/*
 * A245288  a(n) = (4*n^2 - 2*n - 1 + (2*n^2 - 2*n + 1)*(-1)^n)/16.
 * https://oeis.org/A245288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^2 - 2*n - 1 + (2*n^2 - 2*n + 1)*(-1)^n)/16
 * Range:     u8 n <= 7, u16 n <= 204, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = 4 * sn * sn;      // 4*n^2
    seq_t t2 = 2 * sn * sn;      // 2*n^2
    seq_t t3 = 2 * sn;           // 2*n
    seq_t temp;

    if ((n & 1) == 0) /* n is even */
    {
        /* (4n^2 - 2n - 1) + (2n^2 - 2n + 1) = 4n^2 + 2n^2 - 4n = 6n^2 - 4n
           expressed as t1 + t2 - 2*t3  */
        temp = t1 + t2 - (t3 << 1);
    }
    else /* n is odd */
    {
        /* (4n^2 - 2n - 1) - (2n^2 - 2n + 1) = 2n^2 - 2 = t2 - 2   */
        temp = t2 - 2;
    }
    return temp / 16;
}
