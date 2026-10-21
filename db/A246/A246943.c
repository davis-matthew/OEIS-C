/*
 * A246943  a(4n) = 4*n , a(2n+1) = 8*n+4 , a(4n+2) = 2*n+1.
 * https://oeis.org/A246943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n % 4 == 0 * 4 * n if n % 2 == 1 * n / 2 if n % 4 ==
 *            2
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 3) == 0)               /* n ≡ 0 (mod 4) */
        return (seq_t)n;
    if ((n & 1) == 1)               /* n odd  */
    {
        seq_t tn = (seq_t)n;        /* guard against overflow on very small seq_t */
        return tn * 4;
    }                               /* n ≡ 2 (mod 4) */
    return (seq_t)(n / 2);
}
