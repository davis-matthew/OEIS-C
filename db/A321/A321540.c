/*
 * A321540  3^n with digits rearranged into nondecreasing order.
 * https://oeis.org/A321540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004185(3^n) (digits of 3^n sorted in nondecreasing
 *            order)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* 3^0 = 1, the sorted digits of 1 is 1 */
    if (n == 0) return (seq_t)1;

    /* compute 3^n exactly */
    seq_t power = big_pow((seq_t)3, (seq_t)n);

    /* count decimal digits of the power */
    unsigned char cnt[10] = {0};
    digit_counts(power, cnt);

    /* rebuild the number from sorted digits (0..9) */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d)
    {
        for (unsigned char i = 0; i < cnt[d]; ++i)
        {
            result = result * 10 + d;
        }
    }
    return result;
}
