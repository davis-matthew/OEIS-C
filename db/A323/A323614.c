/*
 * A323614  List of 7-powerful numbers (for the definition of k-powerful see
 *          A323395).
 * https://oeis.org/A323614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=144; a(n)=192+16*(n-2) for n>=2
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based */
    if (n == 1)
        return (seq_t)144;

    /* For n >= 2 */
    seq_t m = (seq_t)n - 2u;      /* m = n-2 */
    return (seq_t)192 + m * 16u;  /* 192 + 16*(n-2) */
}
