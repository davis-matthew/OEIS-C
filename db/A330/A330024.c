/*
 * A330024  a(n) = floor(n/z) where z is the number of zeros in the decimal
 *          expansion of 2^n, and a(n)=0 when z=0.
 * https://oeis.org/A330024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n / z) where z is the number of zeros in the
 *            decimal expansion of 2^n, and a(n) = 0 when z = 0
 * Range:     u8 n <= 273, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute a(n) for all n >= 0.  The function works for any build of
   seq_t and idx_t; it does not rely on big‑integer support in seq_t.  */
seq_t a(idx_t n)
{
    /* Maximum decimal digits of 2^n for n <= 10000 is about 3011.
       4000 bytes are sufficient for all required n. */
    static const int MAX_DIGITS = 4000;
    uint8_t digits[MAX_DIGITS];
    size_t len = 1;
    digits[0] = 1;           /* 2^0 = 1 */

    /* Multiply by 2, n times, storing the number in base‑10 digits
       (least significant digit first). */
    for (idx_t i = 0; i < n; ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < len; ++j) {
            uint16_t temp = (uint16_t)digits[j] * 2 + carry;
            digits[j] = (uint8_t)(temp % 10);
            carry = (uint8_t)(temp / 10);
        }
        if (carry) {
            digits[len++] = carry;
        }
    }

    /* Count zeros in the decimal expansion. */
    size_t zeros = 0;
    for (size_t j = 0; j < len; ++j) {
        if (digits[j] == 0) {
            ++zeros;
        }
    }

    /* Return floor(n / zeros), or 0 if there are no zeros. */
    if (zeros == 0) {
        return (seq_t)0;
    }
    return ((seq_t)n) / ((seq_t)zeros);
}
