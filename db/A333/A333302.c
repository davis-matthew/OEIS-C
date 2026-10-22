/*
 * A333302  Numbers produced by iteratively sorting the digits of the last
 *          number from largest to smallest in base 10 and then doubling,
 *          starting with the number 1.
 * https://oeis.org/A333302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = 2 * sort_descending(a(n-1)) for n > 1 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_digits.h"

#define MAX_DIGITS 30000
static uint8_t digit_buf[MAX_DIGITS];

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t v = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        size_t len;
        get_digits(v, digit_buf, &len);

        unsigned cnt[10] = {0};
        for (size_t j = 0; j < len; ++j)
            ++cnt[digit_buf[j]];

        seq_t sorted = 0;
        for (int d = 9; d >= 0; --d)
        {
            unsigned c = cnt[d];
            while (c--)
                sorted = sorted * 10 + d;
        }

        v = sorted * 2;
    }

    return v;
}
