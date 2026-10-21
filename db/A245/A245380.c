/*
 * A245380  a(n) = (7*n^5+5*n^3)/12.
 * https://oeis.org/A245380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^5+5*n^3)/12
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 93, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t (avoid mixing types in intermediate ops) */
    seq_t s = (seq_t)n;

    /* Compute powers of n */
    seq_t n2 = s * s;   /* n^2 */
    seq_t n3 = n2 * s;  /* n^3 */
    seq_t n5 = n3 * n2; /* n^5 */

    /* Compute the numerator and divide by 12 */
    seq_t num = 7 * n5 + 5 * n3;
    return num / 12;
}
