/*
 * A343028  a(n) = floor(11*n / 3).
 * https://oeis.org/A343028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(11*n / 3)
 * Range:     u8 n <= 69, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor(11*n/3) as 3*n + floor(2*n/3) to avoid overflow */
    seq_t n_seq = (seq_t)n;
    seq_t t   = n_seq / (seq_t)3;            /* n div 3   */
    seq_t r   = n_seq % (seq_t)3;            /* n mod 3   */
    seq_t r2  = (r == (seq_t)2) ? (seq_t)1 : (seq_t)0;  /* floor(2*n mod 3 / 3) */
    return n_seq * (seq_t)3 + (seq_t)2 * t + r2;
}
