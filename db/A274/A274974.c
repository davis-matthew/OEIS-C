/*
 * A274974  Centered octahemioctahedral numbers: a(n) =
 *          (4*n^3+24*n^2+8*n+3)/3.
 * https://oeis.org/A274974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3+24*n^2+8*n+3)/3
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t first to ensure all operations are in the target type */
    seq_t nn = (seq_t)n;
    /* Compute the numerator with seq_t precision */
    seq_t num = (seq_t)4 * nn * nn * nn
              + (seq_t)24 * nn * nn
              + (seq_t)8  * nn
              + (seq_t)3;
    /* Division by 3 is safe because the numerator is always a multiple of 3 */
    return num / (seq_t)3;
}
