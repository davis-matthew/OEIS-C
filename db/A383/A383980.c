/*
 * A383980  Length of shortest path (in Chebyshev distance) that touches all
 *          cells in an n X n grid.
 * https://oeis.org/A383980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, walk
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=3, a(4)=6; For n≥5: if n%3==0:
 *            a(n)=2*n-2 + (n*(n-4))/3 if n%3==1: a(n)=2*n+1 + (n*(n-4)-6)/3
 *            if n%3==2: a(n)=2*n-1 + (n*(n-4)-2)/3 These expressions give the
 *            exact shortest Chebyshev path length
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ull || n == 1ull || n == 2ull)
        return 0;
    if (n == 3ull)
        return 3;
    if (n == 4ull)
        return 6;

    seq_t nn = (seq_t)n;
    if ((n % 3ull) == 0ull)
    {
        /* n % 3 == 0, n >= 6 */
        return (2 * nn - 2) + (nn * (nn - 4)) / 3;
    }
    else if ((n % 3ull) == 1ull)
    {
        /* n % 3 == 1, n >= 7 */
        return (2 * nn + 1) + (nn * (nn - 4) - 6) / 3;
    }
    else
    {
        /* n % 3 == 2, n >= 5 */
        return (2 * nn - 1) + (nn * (nn - 4) - 2) / 3;
    }
}
