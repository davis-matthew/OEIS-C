/*
 * A260714  Row 4 of A260717.
 * https://oeis.org/A260714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 + 30*floor((n-1)/8) + inc[(n-1)%8]
 * Range:     u8 n <= 67, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where inc = {0,4,6,10,16,18,22,24} is the cumulative increments for a 8‑term block.  */
/* This is the row 4 of sequence A260717 and satisfies a(1+(7*(k-1))) = A255414(k). */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 */
    static const seq_t inc[8] = {0, 4, 6, 10, 16, 18, 22, 24};

    idx_t m = n - 1;                /* number of differences to sum */
    idx_t t = m / 8;                /* full 8‑term blocks */
    idx_t r = m % 8;                /* remaining differences within a block */

    return (seq_t)7 + (seq_t)(30 * t) + inc[r];
}
