/*
 * A385298  Numbers whose digits all belong to the same residue class mod 9.
 * https://oeis.org/A385298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 586, u64 n <= 9707,
 *            u128 n <= 9707, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385298:
   a(n) = the nth positive integer (with a(1)=0) whose decimal digits
   all belong to the same residue class modulo 9.
   Computation: group numbers by length L; within each length
   the 8 classes 1…8 give single repeated‑digit numbers,
   and the residue 0 class yields 2^(L‑1) numbers
   formed by a leading 9 followed by L‑1 digits 0 or 9. */

#include "../utils/pow10.h"   /* provides seq_t pow10(idx_t) */

seq_t a(idx_t n)
{
    /* a(1) = 0 */
    if (n == 1)
        return 0;

    /* index among positive numbers (a(2) is the first) */
    idx_t rem = n - 2;          /* 0‑based index */

    for (int len = 1; ; ++len)
    {
        /* count of valid numbers of this length */
        idx_t cnt_len = 8 + ((idx_t)1 << (len - 1));

        if (rem < cnt_len)
        {
            /* offset within this length group */
            idx_t offset = rem;

            if (offset < 8)
            {
                /* residue class 1 … 8 (single repeated digit) */
                seq_t k = (seq_t)(offset + 1);          /* digit value */
                seq_t p10 = pow10((idx_t)len);         /* 10^len */
                seq_t num = (p10 - 1) / 9;             /* (10^len-1)/9 */
                return num * k;
            }
            else
            {
                /* residue class 0: digits {0,9} with leading 9 */
                idx_t j = offset - 8;                  /* 0 … 2^(len-1)-1 */
                seq_t num = 9;                         /* leading 9 */

                /* process remaining len-1 digits from most to least
                   significant, mapping bit 0 -> 0, bit 1 -> 9 */
                for (int pos = len - 2; pos >= 0; --pos)
                {
                    unsigned bit = (unsigned)((j >> pos) & 1);
                    seq_t digit = bit ? 9 : 0;
                    num = num * 10 + digit;
                }
                return num;
            }
        }
        else
        {
            rem -= cnt_len;   /* move to next length group */
        }
    }
}
