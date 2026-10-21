/*
 * A263313  Permutation of the nonnegative integers: [4k+3, 4k, 4k+1, 4k+2,
 *          ...].
 * https://oeis.org/A263313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4k+3 for n=4k; a(n) = 4k for n=4k+1; a(n) = 4k+1 for
 *            n=4k+2; a(n) = 4k+2 for n=4k+3. Equivalent to: a(n) = n+3 if
 *            n%4==0, otherwise a(n)=n-1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 3) == 0)          /* n % 4 == 0 */
        return (seq_t)n + 3;
    else
        return (seq_t)n - 1;
}
