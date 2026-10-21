/*
 * A276704  Records in A249859.
 * https://oeis.org/A276704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A276704 – Records in A249859
 *
 * Formula:
 *   a(1) = 3
 *   a(2) = 5
 *   a(3) = 6
 *   for n > 3:  a(n) = 4*n^2 - 12*n + 5
 * This implements the closed form, which works for all n ≥ 1.
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)6;
    seq_t tn = (seq_t)n;
    /* compute 4*tn*tn - 12*tn + 5 */
    return (seq_t)4 * tn * tn - (seq_t)12 * tn + (seq_t)5;
}
