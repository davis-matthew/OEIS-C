/*
 * A370560  Periodic sequence S_{30} of length 28: used to compute the number
 *          that the day 30 of a month occurs on a day of the week for the
 *          Gregorian cycle of 400 years.
 * https://oeis.org/A370560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic sequence of period 28:
 *            3,2,1,2,1,2,2,2,2,1,1,2,2,1,2,1,1,2,2,1,1,1,2,2,1,2,1,1 (i.e.,
 *            a(n) = a(n-28) for n>=28)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[28] = {
        3, 2, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 2, 1, 1, 2, 2, 1,
        1, 1, 2, 2, 1, 2, 1, 1
    };
    return period[n % 28];
}
