/*
 * A273182  a(n) is the second number in a triple consisting of 3 numbers,
 *          which when squared are part of a right diagonal of a magic square
 *          of squares.
 * https://oeis.org/A273182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 14, a(1) = 84, a(n) = 6*a(n-1) - a(n-2) for n ≥ 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 14;
    if (n == 1) return 84;

    seq_t prev = 14;   /* a(n-2) */
    seq_t curr = 84;   /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = curr * 6 - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
