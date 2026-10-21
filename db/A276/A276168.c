/*
 * A276168  a(n) is the minimum first-player score difference of a "Coins in a
 *          Row" game over all permutations of coins 1..n with both players
 *          using a minimax strategy.
 * https://oeis.org/A276168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, more
 * Formula:   for odd n, a(n) = 1 - ((n-1)/2)^2; for even n, a(n) = ( (n/2)
 *            mod 2 )
 * Range:     i8 n <= 16, i16 n <= 16, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) != 0)            /* n is odd */
    {
        seq_t k = (seq_t)((n - 1) >> 1);
        return 1 - k * k;
    }
    else                          /* n is even */
    {
        seq_t k = (seq_t)(n >> 1);
        return (k & 1) ? 1 : 0;
    }
}
