/*
 * A276486  a(1)=1, a(2)=2; thereafter, denoting x=a(n-1)+a(n-2), we have
 *          a(n)=3x+1 if x is odd, otherwise a(n)=x/2^m where 2^m is the
 *          maximal power of 2 dividing x.
 * https://oeis.org/A276486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2; for n>2, let x=a(n-1)+a(n-2); if x is odd →
 *            a(n)=3x+1, else → a(n)=x / 2^v2(x), where v2(x) is the maximal
 *            exponent such that 2^v2(x) divides x
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)2;   /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t x = prev1 + prev2;

        if ((x & (seq_t)1) != 0)          /* x is odd */
        {
            cur = x * (seq_t)3 + (seq_t)1;
        }
        else                              /* x is even */
        {
            seq_t y = x;
            while ((y & (seq_t)1) == 0)   /* strip trailing 0 bits */
                y >>= 1;
            cur = y;
        }

        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
