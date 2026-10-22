/*
 * A339573  a(n) = floor(n*(n+1)/6) - 1.
 * https://oeis.org/A339573
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+1)/6) - 1
 * Range:     u8 n <= 15, u16 n <= 338, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)            /* values below the offset are irrelevant */
        return 0;

    seq_t tn   = (seq_t)n;            /* cast index to seq_t */
    seq_t prod = tn * (tn + (seq_t)1);/* n*(n+1) */
    seq_t res  = prod / (seq_t)6;     /* floor division by 6 */
    return res - (seq_t)1;            /* subtract 1 */
}
