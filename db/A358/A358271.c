/*
 * A358271  Product of the digits of 3^n.
 * https://oeis.org/A358271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of digits of 3^n
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 26, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the product of the digits of 3^n.
   The calculation is done entirely with decimal digits stored in an
   array, preventing overflow of intermediate values.  The
   product is accumulated in a seq_t, which may be arbitrary‑precision
   in the C++ build or a fixed width integer in the C build. */
seq_t a(idx_t n)
{
    /* maximum number of decimal digits of 3^10000 ≈ 4772 */
    enum { MAX_DIGITS = 5000 };
    unsigned char digits[MAX_DIGITS];
    size_t len = 1;            /* current number of digits (least significant first) */
    digits[0] = 1;             /* 3^0 = 1 */

    /* multiply successively by 3, n times */
    for (idx_t i = 0; i < n; ++i) {
        unsigned int carry = 0;
        for (size_t j = 0; j < len; ++j) {
            unsigned int temp = (unsigned int)digits[j] * 3U + carry;
            digits[j] = (unsigned char)(temp % 10U);
            carry     = temp / 10U;
        }
        while (carry) {
            if (len >= MAX_DIGITS)
                break;               /* safety; should never happen for n ≤ 10000 */
            digits[len++] = (unsigned char)(carry % 10U);
            carry /= 10U;
        }
    }

    /* compute product of the digits */
    seq_t prod = 1;
    for (size_t j = 0; j < len; ++j)
        prod *= (seq_t)digits[j];

    return prod;
}
