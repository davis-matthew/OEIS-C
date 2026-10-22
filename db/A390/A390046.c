/*
 * A390046  Circular sorting numbers: a(n) = maximum number of swaps required
 *          to arrange n labeled points on a circle into clockwise order.
 * https://oeis.org/A390046
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(2) = 0 a(3) = 1 For n ≥ 4: a(n) = n - 3 , if n is even or n >
 *            3 and n ≡ 3 (mod 6) a(n) = n - 2 , otherwise This follows from
 *            the known formula for the circular sorting numbers (OEIS
 *            A390046)
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2u)
        return (seq_t)0;
    if (n == 3u)
        return (seq_t)1;

    /* even n */
    if ((n & 1u) == 0u)
        return (seq_t)(n - 3u);

    /* odd n, check n ≡ 3 (mod 6) */
    if ((n % 6u) == 3u)
        return (seq_t)(n - 3u);

    /* all remaining odd n */
    return (seq_t)(n - 2u);
}
