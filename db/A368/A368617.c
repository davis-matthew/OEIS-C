/*
 * A368617  Decimal expansion of 878/323.
 * https://oeis.org/A368617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digit of decimal expansion of 878/323
   (a(1) is the integer part, subsequent terms are the fractional digits).
   The digit can be computed by long division:
   a(1) = 878 / 323
   for n > 1:
   let r = 878 % 323
   for i = 1 .. n-2: r = (r * 10) % 323
   a(n) = (r * 10) / 323
   (All intermediate values fit in idx_t.)
*/
seq_t a(idx_t n)
{
    /* Handle the first digit (integer part) */
    if (n <= 1u)
        return (seq_t)(878u / 323u);

    /* Remainder after computing the integer part */
    idx_t r = 878u % 323u;

    /* Perform (n-2) steps of long division to get the remainder before
       the desired digit */
    idx_t steps = n - 2u;
    for (idx_t i = 0; i < steps; ++i)
        r = (r * 10u) % 323u;

    /* Compute the desired digit */
    idx_t digit = (r * 10u) / 323u;
    return (seq_t)digit;
}
