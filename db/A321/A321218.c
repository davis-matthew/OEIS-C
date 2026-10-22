/*
 * A321218  Decimal expansion of number of Pascals (Pa) in 1 Torr.
 * https://oeis.org/A321218
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit of 101325 Pa / 760 at position n (offset
 *            3). The number 101325/760 = 20265/152 =
 *            133.322368421052657894736842… The integer part has 3 digits: 1 3
 *            3 (positions 3‑5). The decimal expansion has a pre‑period of 3
 *            digits (3,2,2) followed by a repeating cycle of 18 digits:
 *            3,6,8,4,...
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* digits before the decimal point (offset 3) */
    static const unsigned char int_digits[3] = {1, 3, 3};
    /* pre–period of the fractional part */
    static const unsigned char pre[3] = {3, 2, 2};
    /* repeating cycle of the fractional part */
    static const unsigned char cycle[18] = {3, 6, 8, 4, 2, 1, 0, 5,
                                            2, 6, 3, 1, 5, 7, 8, 9,
                                            4, 7};

    /* Sequence is defined for n ≥ 3.  For n < 3 the value is
       irrelevant; return 0 in that case. */
    if (n < 3)
        return (seq_t)0;

    /* Positions 3,4,5 are the integer part. */
    if (n < 6)        /* n == 3,4,5 */
        return (seq_t)int_digits[n - 3];

    /* Decimal positions start at n = 6 (first decimal digit). */
    idx_t dec_pos = n - 6;          /* zero‑based index of the decimal digit */

    if (dec_pos < 3)                /* pre‑period 3 digits */
        return (seq_t)pre[dec_pos];

    /* Remaining digits are periodic with period 18. */
    idx_t cyc_pos = (dec_pos - 3) % 18;
    return (seq_t)cycle[cyc_pos];
}
