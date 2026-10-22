/*
 * A391075  a(n) = 25*n+1 if n is odd and floor((n+6)/8) if n is even.
 * https://oeis.org/A391075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n+1 if n is odd, floor((n+6)/8) if n is even
 * Range:     u8 n <= 10, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1ULL) /* odd */
    {
        /* a(n) = 25*n + 1 */
        return (seq_t)25 * (seq_t)n + (seq_t)1;
    }
    else /* even */
    {
        /* a(n) = floor((n+6)/8) */
        return (seq_t)((n + (idx_t)6) / (idx_t)8);
    }
}
