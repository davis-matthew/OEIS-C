/*
 * A368476  Decimal expansion of 109/65, being the highest possible win/loss
 *          points ratio in a completed 3-set tennis match, with 10-point
 *          final tie-break, which the player loses.
 * https://oeis.org/A368476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368476: decimal digits of 109/65 (including the integer part).
   a(n) = floor( (109 * 10^(n-1)) / 65 ) mod 10
*/
seq_t a(idx_t n)
{
    /* integer part of the fraction */
    seq_t digit = 109 / 65;
    if (n == 1)
        return digit;

    /* remainder after the integer part */
    seq_t rem = 109 % 65;

    /* compute successive decimal digits */
    for (idx_t i = 2; i <= n; ++i) {
        rem = rem * 10;          /* shift remainder */
        digit = rem / 65;        /* next decimal digit */
        rem   = rem % 65;        /* store new remainder */
    }
    return digit;
}
