/*
 * A303276  Decimal expansion of the value of 1 US gallon in liters.
 * https://oeis.org/A303276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, fini, full
 * Formula:   a(n) = nth decimal digit of the exact value 3.785411784; a(n) =
 *            0 for n > 10
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* digits of 3.785411784 (10 digits) */
    static const char digits[] = "3785411784";
    /* sizeof(digits)-1 == 10 */
    if (n <= (idx_t)(sizeof(digits) - 1))
        return (seq_t)(digits[n - 1] - '0');
    return 0;
}
