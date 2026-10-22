/*
 * A303817  Decimal expansion of 360/17.
 * https://oeis.org/A303817
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digits of 360/17. a(2) = 2, a(3) = 1, and for n >= 4 the
 *            digits repeat with period 16: [1,7,6,4,7,0,5,8,8,2,3,5,2,9,4,1]
 *            (the decimal part of 360/17)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the integer part digits */
    if (n == 2) return 2;
    if (n == 3) return 1;
    /* Decimal part repeats every 16 digits */
    static const seq_t period[16] = {
        1, 7, 6, 4, 7, 0, 5, 8,
        8, 2, 3, 5, 2, 9, 4, 1
    };
    /* Compute offset into the period */
    idx_t d = n - 4;          /* zero‑based index of the decimal digit */
    return period[d % 16];
}
