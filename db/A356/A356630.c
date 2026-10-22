/*
 * A356630  a(n) = n! * Sum_{k=0..floor(n/4)} (n - 4*k)^k/(n - 4*k)!.
 * https://oeis.org/A356630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/4⌋} (n - 4*k)^k / (n - 4*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by direct summation with incremental factorial ratios */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t sum   = (seq_t)0;
    seq_t ratio = (seq_t)1;          /* ratio = n! / (n-4k)! initially for k=0 gives 1 */
    idx_t maxk  = n / 4;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* current m = n - 4*k */
        idx_t m_idx = n - 4 * k;
        seq_t m = (seq_t)m_idx;

        /* m^k */
        seq_t pow_k = seq_pow(m, k);

        /* term = ratio * m^k */
        sum += ratio * pow_k;

        /* prepare ratio for next k: multiply by (m-3)*(m-2)*(m-1)*m */
        if (k < maxk)
        {
            seq_t t1 = m - (seq_t)3;
            seq_t t2 = m - (seq_t)2;
            seq_t t3 = m - (seq_t)1;
            seq_t t4 = m;          /* m itself */

            ratio *= t1;
            ratio *= t2;
            ratio *= t3;
            ratio *= t4;
        }
    }

    return sum;
}
