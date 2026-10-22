/*
 * A338206  Inverse of permutation in A160016.
 * https://oeis.org/A338206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n if n is odd; otherwise nearest integer to (2*n)/3
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even index: nearest integer to 2*n/3 is (2*n + 1)/3. */
    if (n & 1)
        return (seq_t)n * 2;
    else
        return ((seq_t)n * 2 + 1) / 3;
}
