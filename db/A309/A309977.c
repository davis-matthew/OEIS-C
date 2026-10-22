/*
 * A309977  Position of n in row n of McGarvey's array; see A007062.
 * https://oeis.org/A309977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of n in row n of McGarvey's array (A007062)
 *            Computation: start with position = n in the identity row; for k
 *            = 2..n reverse each block of length k: the new position is
 *            block_start + (k-1)-offset. After all reversals, the final
 *            position is a(n)
 * Range:     u8 n <= 43, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for n=0, not used in OEIS offset */
    idx_t pos = n;                  /* current position of the element n */
    for (idx_t k = 2; k <= n; ++k) {
        idx_t block_start = ((pos - 1) / k) * k + 1;  /* start of block containing pos */
        idx_t offset = pos - block_start;             /* offset within the block */
        pos = block_start + (k - 1) - offset;         /* position after reversing the block */
    }
    return (seq_t)pos;
}
