/*
 * A277601  (1/2)*A277595.
 * https://oeis.org/A277601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A277595(n)/2, where A277595 = numbers whose last nonzero
 *            digit is 8 (found by search)
 * Range:     u8 n <= 27, u16 n <= 6864, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8137 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A277595_a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = 1;
    for (;; cur = cur + 1)
    {
        seq_t tmp = cur;
        while ((tmp % 10) == 0)
            tmp = tmp / 10;
        if ((tmp % 10) == 8)
        {
            count = count + 1;
            if (count == n)
                return cur;
        }
    }
}

seq_t a(idx_t n)
{
    return A277595_a(n) / 2;
}
