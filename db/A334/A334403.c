/*
 * A334403  Harshad numbers with sum of digits equal to 18.
 * https://oeis.org/A334403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th positive even integer with digit sum 18 (i.e.,
 *            Harshad numbers whose sum of decimal digits equals 18)
 * Range:     u8 n <= 1, u16 n <= 2022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Iterate over multiples of 18; every multiple is even and
       automatically divisible by 9 (since 18 * k has digit sum
       divisible by 9).  We only need to check the digit sum
       equals 18. */
    idx_t count = 0;
    for (idx_t i = 1; ; ++i)
    {
        seq_t num = ((seq_t)i) * ((seq_t)18);
        if (digit_sum(num) == (seq_t)18)
        {
            ++count;
            if (count == n)
                return num;
        }
    }
}
