/*
 * A275709  a(n) = 2*n^3 + 3*n^2.
 * https://oeis.org/A275709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^3 + 3*n^2
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1289, u64 n <= 3030,
 *            u128 n <= 3030, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type once. */
    seq_t nn = (seq_t)n;
    /* Compute n^2 once to avoid recomputation. */
    seq_t nn2 = nn * nn;
    /* a(n) = 2*n^3 + 3*n^2 = 2*nn2*nn + 3*nn2 */
    seq_t res = (seq_t)2 * nn2 * nn + (seq_t)3 * nn2;
    return res;
}
