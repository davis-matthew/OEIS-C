/*
 * A392477  A modified Sisyphus function: a(n) = concatenation of (number of
 *          odd digits in n) (number of even digits in n) (number of digits in
 *          n).
 * https://oeis.org/A392477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenation of (number of odd digits in n) (number of
 *            even digits in n) (number of digits in n)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static inline int digits_of_uint64(idx_t x)
{
    if (x == 0) return 1;
    int cnt = 0;
    while (x)
    {
        cnt++;
        x /= 10;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t odd  = 0;
    idx_t even = 0;
    idx_t dign = 0;

    if (tmp == 0)
    {
        odd  = 0;
        even = 1;
        dign = 1;
    }
    else
    {
        while (tmp)
        {
            int d = tmp % 10;
            if (d & 1)
                odd++;
            else
                even++;
            dign++;
            tmp /= 10;
        }
    }

    seq_t result;
    if (odd > 0)
    {
        result = (seq_t)odd;
        result = result * pow10(digits_of_uint64(even)) + (seq_t)even;
    }
    else
    {
        result = (seq_t)even;
    }
    result = result * pow10(digits_of_uint64(dign)) + (seq_t)dign;
    return result;
}
