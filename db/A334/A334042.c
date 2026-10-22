/*
 * A334042  Write n^2 in binary, interchange 0's and 1's, convert back to
 *          decimal.
 * https://oeis.org/A334042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A035327(n^2) (bitwise complement of the binary *
 *            representation of n^2). That is, write n^2 in binary, *
 *            interchange 0's and 1's, and convert back to decimal
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the binary complement of n^2 using only allowed operations. */
seq_t a(idx_t n)
{
    /* Compute n^2 in a type wide enough to hold the full value. */
    idx_t square = (idx_t)n * (idx_t)n;

    /* Special case: n^2 = 0  →  binary "0" flips to "1". */
    if (square == 0)
        return (seq_t)1;

    /* Determine the number of bits required to represent square. */
    unsigned L = 0;
    idx_t tmp = square;
    while (tmp) {
        tmp >>= 1;
        ++L;
    }

    /* Build a mask with L low bits set to 1.  L ≤ 64 because square fits
       in an unsigned 64‑bit integer. */
    idx_t mask = ((idx_t)1 << L) - 1;

    /* Complement only the lower L bits. */
    idx_t flipped = (~square) & mask;

    /* Cast back to the sequence type.  The result is guaranteed to
       fit in seq_t when it should (per the problem statement). */
    return (seq_t)flipped;
}
