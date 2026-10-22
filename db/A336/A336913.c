/*
 * A336913  Image of n under the 3^x+1 map, which is a variation of the 3x+1
 *          (Collatz) map.
 * https://oeis.org/A336913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log_2(n)) if n is even, 3^n+1 if n is odd
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) == 0)   /* n is even */
    {
        idx_t temp = n;
        idx_t logval = 0;
        while (temp > 1)
        {
            temp >>= 1;
            logval++;
        }
        return (seq_t)logval;
    }
    else                       /* n is odd */
    {
        seq_t pow_val = big_pow((seq_t)3, (seq_t)n);
        return pow_val + (seq_t)1;
    }
}
