/*
 * A366410  Number of linearly independent solutions to the neighbor sum
 *          problem on a cubical (3n-1) X (3n-1) X (3n-1) chessboard.
 * https://oeis.org/A366410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 if n is odd and 5 ∤ n * a(n) = 6 if n is odd and 5 | n
 *            * a(n) = 15 if n is even and n ≡ 0 (mod 4) * a(n) = 9 if n is
 *            even, n ≡ 2 (mod 4) and 5 | n * a(n) = 3 if n is even, n ≡ 2
 *            (mod 4) and 5 ∤ n * This reproduces the sequence A366410
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is >= 1, but handle n==0 gracefully */
    if (n == 0)
        return (seq_t)0;
 
    idx_t nn = n;           /* local copy for readability */
    if ((nn & 1ULL) == 1ULL)  /* odd n */
    {
        if ((nn % 5ULL) == 0ULL)
            return (seq_t)6;
        else
            return (seq_t)0;
    }
    else                     /* even n */
    {
        if ((nn % 4ULL) == 0ULL)
            return (seq_t)15;
        else /* nn % 4 == 2 */
        {
            if ((nn % 5ULL) == 0ULL)
                return (seq_t)9;
            else
                return (seq_t)3;
        }
    }
}
