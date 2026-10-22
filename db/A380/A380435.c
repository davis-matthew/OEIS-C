/*
 * A380435  Erase digit 0 from decimal expansion of n. Then repeatedly apply
 *          the number of divisor function (A000005) onto each digit until a
 *          stationary value is reached. a(n) is the final stationary value
 *          (if it is reached for all digits).
 * https://oeis.org/A380435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate digits of n after erasing 0s, mapping a digit
 *            d to 1 if d==1 else to 2
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* gather digits in reverse order */
    unsigned digits[20];          /* 20 is enough for uint64_t */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned)(tmp % 10);
        tmp /= 10;
    }
    if (len == 0) /* n==0 never occurs for n>=1 */
        return 0;

    seq_t result = 0;
    /* process from most significant to least significant digit */
    for (int i = len - 1; i >= 0; --i) {
        unsigned d = digits[i];
        if (d == 0)
            continue;               /* erase zeros */
        unsigned mapped = (d == 1) ? 1 : 2;   /* map remaining digits */
        result = result * 10 + mapped;
    }
    return result;
}
