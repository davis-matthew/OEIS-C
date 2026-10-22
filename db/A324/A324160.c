/*
 * A324160  Number of zero-containing nonnegative integers <= n.
 * https://oeis.org/A324160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n+1) - C(n) where C(n) is the number of integers in
 *            [0,n] that contain no digit '0'. C(n) is computed by traversing
 *            the decimal digits of n from most significant to least, counting
 *            how many smaller prefixes are possible without using the digit
 *            '0' and handling the tight prefix exactly
 * Range:     u8 n <= 1073, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Length of n in decimal digits */
    uint8_t len = 0;
    idx_t temp = n;
    do {
        ++len;
        temp /= 10;
    } while (temp);

    /* Precompute powers of 9 and 10 up to len */
    idx_t pow9[21], pow10[21];   /* len <= 20 for 64-bit n */
    pow9[0] = 1;
    pow10[0] = 1;
    for (uint8_t i = 1; i <= len; ++i) {
        pow9[i] = pow9[i-1] * 9U;
        pow10[i] = pow10[i-1] * 10U;
    }

    /* Count numbers with fewer digits than n that have no zero */
    idx_t count_no_zero = 0;
    for (uint8_t d = 1; d < len; ++d)
        count_no_zero += pow9[d];

    /* Process numbers with the same number of digits as n */
    bool tight = true;
    temp = n;
    for (uint8_t pos = 0; pos < len; ++pos) {
        uint8_t power = len - 1 - pos;          /* distance from right */
        idx_t digit = (temp / pow10[power]) % 10U;
        uint8_t remaining = len - pos - 1;      /* digits to the right */

        if (tight) {
            /* Count all numbers where the current digit is < 'digit'
               (but not zero) and the rest of the digits are unrestricted. */
            for (idx_t d = 1; d < digit; ++d)
                count_no_zero += pow9[remaining];

            if (digit == 0) {
                tight = false;   /* cannot proceed further (n itself has a 0) */
                break;
            }
        }
    }

    /* If we never hit a zero digit, include n itself */
    if (tight)
        count_no_zero += 1U;

    /* a(n) = total numbers in [0,n] minus those without a zero */
    return (seq_t)((idx_t)n + 1U - count_no_zero);
}
