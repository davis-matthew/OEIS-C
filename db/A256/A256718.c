/*
 * A256718  a(n) = n*(n+1)*(7*n-6)/2.
 * https://oeis.org/A256718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n-6)/2
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 1);
    res = res * (((seq_t)7 * nn) - (seq_t)6);
    res /= (seq_t)2;
    return res;
}
