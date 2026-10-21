/*
 * A259552  a(n) = (1/4)*n^4 - (1/2)*n^3 + (3/4)*n^2 - (1/2)*n + 41.
 * https://oeis.org/A259552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/4)*n^4 - (1/2)*n^3 + (3/4)*n^2 - (1/2)*n + 41
 * Offset: 1  (a(1) is the first term)
 */
seq_t a(idx_t n)
{
    /* Compute numerator: n^4 - 2*n^3 + 3*n^2 - 2*n + 164 */
    seq_t n_val = (seq_t)n;
    seq_t n2 = n_val * n_val;           /* n^2 */
    seq_t n3 = n2 * n_val;              /* n^3 */
    seq_t n4 = n3 * n_val;              /* n^4 */

    seq_t num = n4
              - (seq_t)2 * n3
              + (seq_t)3 * n2
              - (seq_t)2 * n_val
              + (seq_t)164;

    /* Divide by 4 – the division is exact for all integer n */
    return num / (seq_t)4;
}
