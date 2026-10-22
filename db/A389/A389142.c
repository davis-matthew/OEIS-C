/*
 * A389142  Lower (1/3)-midsequence of square numbers and triangular numbers;
 *          see Comments.
 * https://oeis.org/A389142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3*n*n + n + 1)/6)
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the lower (1/3)-midsequence of square numbers and triangular numbers.
 * This function matches the declaration in the prelude which uses C++ linkage,
 * so the definition must not introduce C linkage (i.e. no extern "C").
 * It works for all supported seq_t types: unsigned integer types of any size
 * or an arbitrary‑precision integer class with the usual arithmetic operators. */
seq_t a(idx_t n)
{
    /* Use idx_t for intermediate arithmetic to avoid overflow when seq_t is small. */
    idx_t nn  = n;
    idx_t num = 3*nn*nn + nn + 1;
    idx_t res = num / 6;
    return (seq_t)res;
}
