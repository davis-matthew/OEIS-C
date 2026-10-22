/*
 * A316988  The odd members of the "Almost natural numbers" (A007376).
 * https://oeis.org/A316988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Subsequence: a(n) is the n-th odd digit of Champernowne’s constant
   (decimal concatenation 1,2,3,4,5,6,7,8,9,10,11,12,…). */
seq_t a(idx_t n)
{
    idx_t odd_cnt = 0;
    for (idx_t num = 1; ; ++num)
    {
        /* Gather the decimal digits of num into a stack (least‑significant first). */
        idx_t temp = num;
        int digits[20];
        int d = 0;
        while (temp > 0)
        {
            digits[d++] = (int)(temp % 10);
            temp /= 10;
        }

        /* Emit digits in correct order and count the odd ones. */
        for (int i = d - 1; i >= 0; --i)
        {
            seq_t digit = (seq_t)digits[i];
            if ((digit & 1) != 0)           /* digit is odd */
            {
                ++odd_cnt;
                if (odd_cnt == n)
                    return digit;
            }
        }
    }
}
