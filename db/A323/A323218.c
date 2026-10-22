/*
 * A323218  a(n) = (4*n^3 + 30*n^2 + 50*n)/3 + 1.
 * https://oeis.org/A323218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 30*n^2 + 50*n)/3 + 1
 * Range:     u8 n <= 2, u16 n <= 22, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to ensure all arithmetic is in the output type. */
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;             /* n^2 */
    seq_t N3 = N2 * N;            /* n^3 */
    seq_t num = 4 * N3 + 30 * N2 + 50 * N; /* numerator of the fraction */

    return num / 3 + 1;          /* exact division by 3 followed by +1 */
}
