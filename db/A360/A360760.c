/*
 * A360760  a(n) = n^16 + n^15 + n^2 + 1.
 * https://oeis.org/A360760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^16 + n^15 + n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 255,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t N = (seq_t)n;      /* cast the index once */

    /* n^2 */
    seq_t n2 = N * N;

    /* compute n^15 (by repeated multiplication) */
    seq_t p = N;
    for (int i = 1; i < 15; ++i)
        p *= N;
    seq_t n15 = p;

    /* n^16 = n^15 * n */
    seq_t n16 = n15 * N;

    /* final sum */
    return n16 + n15 + n2 + 1;
}
