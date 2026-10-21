/*
 * A270993  Values of A076336(n) such that A076336(n) = A076336(n+1) - 14.
 * https://oeis.org/A270993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7523267 + 11184810*(n-1)
 * Range:     u8 none, u16 none, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined by a linear formula */
    const seq_t offset = (seq_t)7523267;
    const seq_t step   = (seq_t)11184810;
    if (n == 0) {
        /* undefined by the OEIS offset, but we avoid underflow */
        return offset;
    }
    return offset + (seq_t)(n - 1) * step;
}
