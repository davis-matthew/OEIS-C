/*
 * A370559  Periodic sequence S_{29} of length 28: used to compute the number
 *          of times the 29th day of a month occurs on a day of the week for
 *          the Gregorian cycle of 400 years.
 * https://oeis.org/A370559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = periodic sequence of length 28: repeat(1, 2, 2, 1, 2, 1,
 *            2, 2, 1, 2, 1, 1, 3, 2, 1, 2, 1, 2, 2, 2, 2, 1, 1, 2, 2, 1, 2,
 *            1)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is strictly periodic with period 28. */
    static const seq_t period[28] = {
        1, 2, 2, 1, 2, 1, 2, 2,
        1, 2, 1, 1, 3, 2, 1, 2,
        1, 2, 2, 2, 2, 1, 1, 2,
        2, 1, 2, 1
    };

    /* Compute n modulo the period length (0..27) and return the value. */
    idx_t idx = n % (idx_t)28;
    /* idx fits into an unsigned int; safety to cast for array indexing. */
    return period[(unsigned int)idx];
}
