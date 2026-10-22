/*
 * A305265  a(n) = 12*2^n + 62.
 * https://oeis.org/A305265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*2^n + 62. Recurrence: a(n) = 3*a(n-1) - 2*a(n-2),
 *            a(0)=74, a(1)=86
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;
    idx_t i = n;
    while (i--) /* compute 2^n in val */
        val *= (seq_t)2;

    val = val * (seq_t)12 + (seq_t)62; /* a(n) = 12*2^n + 62 */
    return val;
}
