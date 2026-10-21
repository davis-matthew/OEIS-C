/*
 * A242782  The longest Dewey Decimal number (as of 2001).
 * https://oeis.org/A242782
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  cons, dumb, nonn, fini, full
 * Formula:   a(n) is the (n‑2)th decimal digit of the longest Dewey Decimal
 *            number known (301.1543012917492705694), ignoring the decimal
 *            point
 * Range:     u8 n <= 24, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Decimal digits of 301.1543012917492705694, concatenated. */
    static const seq_t digits[] = {
        3, 0, 1, 1, 5, 4, 3, 0, 1, 2,
        9, 1, 7, 4, 9, 2, 7, 0, 5, 6,
        9, 4
    };
    static const size_t len = sizeof(digits) / sizeof(digits[0]);

    /* Sequence is defined for n ≥ 3. */
    if (n < 3 || n - 3 >= len)
        return (seq_t)0;          /* out of range → 0 */

    return digits[n - 3];
}
