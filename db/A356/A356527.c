/*
 * A356527  Decimal expansion of number of millimeters of water (mmH2O) in 1
 *          millimeter of mercury (mmHg).
 * https://oeis.org/A356527
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth decimal digit of the constant 13.5951
 * (offset 2, so a(2)=1, a(3)=3, a(4)=5, a(5)=9, a(6)=5, a(7)=1).
 * For n > 7 the decimal expansion terminates, so the digits are 0. */
seq_t a(idx_t n)
{
    /* Values before the offset are undefined; return 0 */
    if (n < 2U)
        return (seq_t)0;

    /* The six known digits of 13.5951: 1 3 5 9 5 1 */
    static const seq_t digits[6] = { 1, 3, 5, 9, 5, 1 };

    /* n-2 is in [0,5] for the known digits */
    if (n <= 7U) /* 2 <= n <= 7 */
        return digits[n - 2U];

    /* All further digits are zeros */
    return (seq_t)0;
}
