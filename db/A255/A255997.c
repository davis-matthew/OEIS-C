/*
 * A255997  Number of length n+6 0..1 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n^3 + 533*n)/6) + 3*n^2 + 28 for n > 4 (with the
 *            initial values a(1)=128, a(2)=256, a(3)=354, a(4)=451)
 * Range:     u8 n <= 1, u16 n <= 65, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)128;
    if (n == 2) return (seq_t)256;
    if (n == 3) return (seq_t)354;
    if (n == 4) return (seq_t)451;
    if (n < 1)  return (seq_t)0;    /* undefined below the offset */

    /* compute the polynomial for n > 4 */
    seq_t ns = (seq_t)n;
    seq_t term1 = (ns * ns * ns + (seq_t)533 * ns) / 6;   /* (n^3 + 533n)/6 */
    seq_t term2 = (seq_t)3 * ns * ns;                     /* 3n^2 */
    return term1 + term2 + (seq_t)28;                     /* +28 */
}
