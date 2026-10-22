/*
 * A357146  a(n) = n! * Sum_{k=0..floor(n/2)} (n - 2*k)^(2*k)/(n - 2*k)!.
 * https://oeis.org/A357146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * sum_{k=0..⌊n/2⌋} (n-2k)^(2k)/(n-2k)!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;          /* accumulated sum of terms      */
    idx_t  maxk = n / 2;    /* highest k such that n-2k >= 0  */

    seq_t f   = 1;          /* product n!/(n-2k)!            */
    seq_t pow = 1;          /* (n-2k)^(2k)                    */
    idx_t j   = n;          /* current value of n-2k          */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* Add current term to the sum */
        sum += f * pow;

        /* No more terms to process? */
        if (k == maxk)
            break;

        /* Update f: multiply by the two factors (n-2k) and (n-2k-1) */
        f *= j;          /* multiply by n-2k   */
        f *= (j - 1);    /* multiply by n-2k-1 */

        /* Prepare for next k (n-2(k+1) = j-2) */
        j -= 2;                                 /* new n-2(k+1)  */
        pow = big_pow((seq_t)j, (seq_t)(2 * (k + 1))); /* new power   */
    }

    return sum;
}
