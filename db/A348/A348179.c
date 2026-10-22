/*
 * A348179  Replace each decimal digit d of n with the digit that is d steps
 *          to the right of d. Interpret the digits of n as a cycle: one step
 *          to the right from the last digit is considered to be the first.
 * https://oeis.org/A348179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each decimal digit d of n, replace it with the digit that
   is d steps to the right in the cyclic order of the digits of n. */
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Determine the number of decimal digits (at least 1). */
    idx_t t = n;
    size_t k = 0;
    while (t > 0) {
        ++k;
        t /= 10;
    }
    if (k == 0)       /* n == 0 */
        k = 1;

    /* Store the digits of n (most significant first). */
    uint8_t digits[20];            /* 20 digits suffice for any uint64_t */
    t = n;
    if (t == 0) {
        digits[0] = 0;
    } else {
        for (size_t i = k; i > 0; --i) {
            digits[i - 1] = (uint8_t)(t % 10);
            t /= 10;
        }
    }

    /* Build the result by applying the cyclic shift. */
    seq_t result = 0;
    for (size_t i = 0; i < k; ++i) {
        uint8_t d = digits[i];
        size_t newpos = (i + d) % k;
        uint8_t nd = digits[newpos];
        result = result * 10 + (seq_t)nd;
    }

    return result;
}
