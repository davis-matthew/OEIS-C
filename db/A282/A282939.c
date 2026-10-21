/*
 * A282939  Maximum number of straight lines required to draw the boundary of
 *          any polyomino with n squares.
 * https://oeis.org/A282939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n for n mod 4 ≠ 1 Formula: a(n) = 2*n + 2 for n mod 4 =
 *            1 This matches the OEIS definition of A282939
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 4 using a bit mask (since 4 = 2²). */
    if ((n & 3) == 1) {
        /* Case n ≡ 1 (mod 4): a(n) = 2*n + 2. */
        return (seq_t)2 * n + 2;
    } else {
        /* All other residue classes: a(n) = 2*n. */
        return (seq_t)2 * n;
    }
}
