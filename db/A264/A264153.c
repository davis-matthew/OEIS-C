/*
 * A264153  a(n) = ((2*n)!)^2 / 2^n.
 * https://oeis.org/A264153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2*n)!)^2 / 2^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1 */
/*            a(n) = a(n-1) * ((2*n)*(2*n-1))^2 / 2  for n >= 1 */

seq_t a(idx_t n)
{
    seq_t result = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)(2 * i) * (seq_t)(2 * i - 1); /* (2n)*(2n-1) */
        term = term * term;          /* square it */
        term = term / 2;             /* divide by 2 – integer result */
        result = result * term;      /* multiply into running product */
    }
    return result;
}
