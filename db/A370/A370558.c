/*
 * A370558  Periodic sequence S_1 of length 28: used to compute the number of
 *          times a day from 1 to 28 of a month occurs on a day of the week
 *          for the Gregorian cycle of 400 years.
 * https://oeis.org/A370558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = repeat(1, 2, 2, 1, 2, 1, 2, 2, 1, 3, 1, 1, 3, 2, 1, 3, 1,
 *            2, 2, 2, 2, 1, 1, 2, 2, 1, 3, 1) (period 28)
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[28] =
    {
        1, 2, 2, 1, 2, 1, 2, 2,
        1, 3, 1, 1, 3, 2, 1, 3,
        1, 2, 2, 2, 2, 1, 1, 2,
        2, 1, 3, 1
    };
    return period[n % 28];
}
