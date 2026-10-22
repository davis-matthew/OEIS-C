/*
 * A350539  Chronological Julian day number of the first day (Muharram 1) of
 *          Tabular Islamic year n.
 * https://oeis.org/A350539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n*10631 + 3)/30) + 1948086
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset 1, a(1) = 1948440) */

seq_t a(idx_t n)
{
    /* compute the formula using only seq_t arithmetic */
    seq_t nn = (seq_t)n;              /* convert index to seq_t */
    seq_t temp = (nn * 10631 + 3) / 30;   /* floor division */
    seq_t result = temp + 1948086;
    return result;
}
