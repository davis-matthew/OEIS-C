/*
 * A348287  Arrange nonzero digits of n in increasing order then append the
 *          zeros.
 * https://oeis.org/A348287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = arrange nonzero digits of n in increasing order then
 *            append zeros
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    unsigned char digits[20];
    int len = 0;
    uint64_t tmp = n;
    int zero_count = 0;

    while (tmp > 0) {
        unsigned char d = (unsigned char)(tmp % 10);
        tmp /= 10;
        if (d == 0)
            ++zero_count;
        else
            digits[len++] = d;
    }

    unsigned char count[10] = {0};
    for (int i = 0; i < len; ++i)
        ++count[digits[i]];

    seq_t res = 0;
    for (unsigned d = 1; d <= 9; ++d) {
        unsigned c = count[d];
        while (c--)
            res = res * 10 + d;
    }

    while (zero_count--)
        res = res * 10;

    return res;
}
