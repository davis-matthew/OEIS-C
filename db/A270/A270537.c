/*
 * A270537  Numbers that are equal to their number of digits multiplied by the
 *          sum the cubes of the digits.
 * https://oeis.org/A270537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   digit_len, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth integer m such that m equals the number of
   digits of m multiplied by the sum of the cubes of its digits. The
   sequence starts at offset 1 (a(1) = 1). */

#include <stddef.h>
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t count = 0;
    for (idx_t m = 1; ; ++m)
    {
        seq_t mval = (seq_t)m;

        idx_t d = digit_len(mval);

        uint8_t buf[64];
        size_t len = 0;
        get_digits(mval, buf, &len);

        seq_t sum_cubes = 0;
        for (size_t i = 0; i < len; ++i)
        {
            seq_t dig = (seq_t)buf[i];
            sum_cubes += dig * dig * dig;
        }

        if ((seq_t)d * sum_cubes == mval)
        {
            ++count;
            if (count == n)
                return mval;
        }
    }
}
