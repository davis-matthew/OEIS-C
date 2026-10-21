/*
 * A264977  a(0) = 0, a(1) = 1, a(2*n) = 2*a(n), a(2*n+1) = a(n) XOR a(n+1).
 * https://oeis.org/A264977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0, a(1)=1, a(2*n)=2*a(n), a(2*n+1)=a(n) XOR a(n+1)
 * Range:     u8 n <= 255, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if ((n & 1ULL) == 0)       /* even */
        return 2 * a(n >> 1);
    else                       /* odd */
    {
        seq_t left  = a(n >> 1);
        seq_t right = a((n >> 1) + 1);
        return left ^ right;
    }
}
