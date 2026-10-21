/*
 * A294732  Maximal diameter of the connected cubic graphs on 2*n vertices.
 * https://oeis.org/A294732
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2) = 1; a(n) = (6*n - 11 - (-1)^n)/4 for n > 2
 * Range:     u8 n <= 172, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed-form formula for OEIS A294732. */
seq_t a(idx_t n)
{
    /* base case for the offset */
    if (n == 2) return (seq_t)1;

    /* compute 6*n using idx_t to avoid overflow on small seq_t */
    idx_t sixn = 6 * n;

    /* subtract 12 for even n, 10 for odd n */
    if ((n & 1) == 0) {
        sixn -= 12;
    } else {
        sixn -= 10;
    }

    /* divide by 4; the result is always an integer */
    return (seq_t)(sixn / 4);
}
