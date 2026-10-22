/*
 * A345889  Number of tilings of an n-cell circular array with rectangular
 *          tiles of any size, and where the number of possible colors of a
 *          tile is given by the smallest cell covered.
 * https://oeis.org/A345889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n+1} k!/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t fact = 1;   /* factorial of k-1 before each iteration */
    for (idx_t k = 2; k <= n + 1; ++k) {
        fact *= (seq_t)k;  /* compute k! */
        sum += fact / 2;   /* add (k!)/2 to the running total */
    }
    return sum;
}
