/*
 * A352434  The number of simple vertices on a diagonal of a regular 2n-gon
 *          when all its vertices are connected by lines and where the
 *          diagonal passes through the center of the 2n-gon.
 * https://oeis.org/A352434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n=2, n-1 if n is odd, n-4 if n is a multiple of 12,
 *            n-2 otherwise
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2)
        return (seq_t)1;
    if (n & 1ULL) /* odd */
        return (seq_t)n - 1;
    if (n % 12 == 0)
        return (seq_t)n - 4;
    return (seq_t)n - 2;
}
