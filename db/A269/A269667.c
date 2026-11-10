/*
 * A269667  a(n) = A270172(10*n).
 * https://oeis.org/A269667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A270172(10*n), where A270172(m) is the number of steps to
 *            reach a single digit by adding 1 to every digit of m mod 10 and
 *            dropping leading zeros (computed by direct simulation)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9948 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t A270172_a(idx_t m)
{
    if (m <= 9)
        return 0;

    unsigned char digits[40];
    size_t len = 0;
    idx_t tmp = m;
    while (tmp != 0) {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    /* reverse so most significant first */
    for (size_t i = 0; i < len / 2; ++i) {
        unsigned char t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    seq_t steps = 0;
    while (len > 1) {
        for (size_t j = 0; j < len; ++j)
            digits[j] = (unsigned char)((digits[j] + 1) % 10);

        size_t skip = 0;
        while (skip + 1 < len && digits[skip] == 0)
            ++skip;
        if (skip > 0) {
            for (size_t j = 0; j + skip < len; ++j)
                digits[j] = digits[j + skip];
            len -= skip;
        }
        ++steps;
    }
    return steps;
}

seq_t a(idx_t n)
{
    return A270172_a(n * 10);
}
