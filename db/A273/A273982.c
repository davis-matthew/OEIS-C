/*
 * A273982  Number of little cubes visible around an n X n X n cube with a
 *          face on a table.
 * https://oeis.org/A273982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^2 - 8*n + 4
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = (seq_t)5 * nn * nn - (seq_t)8 * nn + (seq_t)4;
    return res;
}
