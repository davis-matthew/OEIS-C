/*
 * A330451  a(n) = a(n-3) + 20*n - 30 for n > 2, with a(0)=0, a(1)=3, a(2)=13.
 * https://oeis.org/A330451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10*n^2/3) (equivalent to a(n) = a(n-3)+20*n-30 for
 *            n>2)
 * Range:     u8 n <= 8, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)13;

    /* Compute floor(10*n^2/3) using integer arithmetic. */
    seq_t nn = (seq_t)n;
    /* Square n, then multiply by 10, finally divide by 3 (truncating). */
    return ((seq_t)10 * nn * nn) / (seq_t)3;
}
