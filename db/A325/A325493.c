/*
 * A325493  a(n) = Sum_{k=1..n} A325910(k) * 10^(2^(k-1)).
 * https://oeis.org/A325493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A325910(k) * 10^(2^(k-1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t a_val = 0;                     /* result a(n)          */
    seq_t block_prev2 = 0;               /* A325910(k-2)         */
    seq_t block_prev1 = 0;               /* A325910(k-1)         */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t block_val;                 /* A325910(k)           */

        /* Compute A325910(k) */
        if (k == 1)
        {
            block_val = 1;
        }
        else if (k == 2)
        {
            block_val = 10;
        }
        else
        {
            /* Length of the leading block of 1's: 2^(k-2) */
            idx_t len = ((idx_t)1ULL) << (k - 2);

            seq_t ten_pow_len = pow10(len);                     /* 10^(2^(k-2))             */
            seq_t ones_block  = (ten_pow_len - 1) / 9;           /* 111...1  (len times)     */
            seq_t part = ones_block * ten_pow_len;                /* ones * 10^len             */
            block_val = part + block_prev2;                      /* block = previous + part   */
        }

        /* Add block * 10^(2^(k-1)) to the sum */
        idx_t shift_exp = ((idx_t)1ULL) << (k - 1);              /* 2^(k-1)                  */
        seq_t shift_val = pow10(shift_exp);                     /* 10^(2^(k-1))              */
        a_val += block_val * shift_val;

        /* Slide the window for the next iteration */
        block_prev2 = block_prev1;
        block_prev1 = block_val;
    }

    return a_val;
}
