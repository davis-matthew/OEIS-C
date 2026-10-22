/*
 * A321797  Delete the decimal digits of n that appear exactly once; write 0
 *          if all digits disappear.
 * https://oeis.org/A321797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   delete all decimal digits of n that appear exactly once; leading
 *            zeros are removed unless the result is 0
 * Range:     u8 n <= 332, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    unsigned char freq[10] = {0};
    unsigned char digits[25];
    size_t len = 0;
    idx_t tmp = n;

    if (tmp == 0)
    {
        freq[0] = 1;
        digits[0] = 0;
        len = 1;
    }
    else
    {
        while (tmp > 0)
        {
            unsigned char d = (unsigned char)(tmp % 10);
            freq[d]++;
            digits[len++] = d;
            tmp /= 10;
        }
    }

    seq_t result = (seq_t)0;
    bool started = false;

    for (size_t i = len; i-- > 0; )
    {
        unsigned char d = digits[i];
        if (freq[d] > 1)
        {
            if (started || d != 0)
            {
                result = result * (seq_t)10 + (seq_t)d;
                started = true;
            }
        }
    }

    return result;
}
