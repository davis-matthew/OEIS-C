/*
 * A306392  a(n) = 2^n with 1's and 2's swapped.
 * https://oeis.org/A306392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal representation of 2^n with all digits * 1 and
 *            2 exchanged
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Compute 2^n.  The utility function returns a value of type seq_t. */
    seq_t val = pow2(n);

    /* The following routine extracts the decimal digits of val
     * without using any auxiliary string‐conversion function.  We
     * first count the digits, then extract them in reverse order,
     * swapping 1↔2 along the way. */

    /* Count digits (at least one). */
    seq_t tmp = val;
    size_t digits = 0;
    do
    {
        ++digits;
        tmp /= 10;
    }
    while (tmp > 0);

    /* Allocate buffer for the decimal string (including NUL). */
    char *buf = (char *)malloc(digits + 1);
    if (!buf)
        return val;  /* On allocation failure, return 2^n unchanged. */

    buf[digits] = '\0';

    /* Extract digits in reverse order, swapping 1 and 2. */
    tmp = val;
    for (size_t i = 0; i < digits; ++i)
    {
        seq_t d = tmp % 10;
        tmp /= 10;
        char c = (char)('0' + (int)d);
        if (c == '1')
            c = '2';
        else if (c == '2')
            c = '1';
        buf[digits - 1 - i] = c;
    }

    /* Convert the resulting decimal string back into a seq_t. */
    seq_t result = seq_from_str(buf);

    free(buf);
    return result;
}
