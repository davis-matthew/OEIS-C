/*
 * A368146  Decimal expansion of 174/96 = 29/16, being the highest possible
 *          win/loss points ratio in a completed 5-set tennis match, with
 *          10-point final tie-break, which the player loses.
 * https://oeis.org/A368146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = nth digit of the decimal expansion of 174/96 (i.e. of
 *            29/16 = 1.8125). The sequence consists of the digits 1,8,1,2,5
 *            followed by an infinite string of zeros
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t numer = (seq_t)174;
    const seq_t denom = (seq_t)96;
    seq_t rem   = numer;  /* remainder before extracting a digit */
    seq_t digit = 0;

    for (idx_t i = 1; i <= n; ++i) {
        digit = rem / denom;          /* current decimal digit   */
        rem   = (rem % denom) * 10;    /* remainder for next digit */
    }
    return digit;
}
