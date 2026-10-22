/*
 * A392730  Odd squares k such that k has an odd number of digits and the sum
 *          of digits of k is an odd number.
 * https://oeis.org/A392730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 6191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_sum, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search method: iterate odd squares and keep ones with odd digit length and odd digit sum. */

#include "../utils/mul_small.h"
#include "../utils/digit_len.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    idx_t m   = 1;          /* start with the first odd integer */

    while (1)
    {
        /* square m (seq_t) */
        seq_t m_seq = (seq_t)m;
        seq_t sq = mul_small(m_seq, (unsigned)m);

        /* odd number of digits? */
        if (digit_len(sq) & 1U)
        {
            /* odd sum of digits? */
            if ((digit_sum(sq) & 1U) != 0U)
            {
                ++cnt;
                if (cnt == n)
                    return sq;
            }
        }
        m += 2; /* next odd integer */
    }
}
