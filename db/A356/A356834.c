/*
 * A356834  a(n) = n! * Sum_{k=0..floor(n/2)} (n - 2*k)^n/(n - 2*k)!.
 * https://oeis.org/A356834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..⌊n/2⌋} (n-2k)^n / (n-2k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    idx_t m   = n;            /* current term (n-2k) */
    seq_t factor = 1;          /* n! / m!   for current m */
    seq_t sum    = 0;          /* running total */

    /* iterate over m = n, n-2, n-4, ... with same parity as n */
    for (;;)
    {
        if (((m & 1u) == (n & 1u)) && m <= n) /* same parity */
        {
            seq_t pow_val = pow_uint(m, n);   /* m^n  */
            sum += factor * pow_val;
        }

        if (m <= 1)            /* finished the last term */
            break;

        /* Prepare factor for m-2:
           factor' = n! / (m-2)! = factor * m * (m-1) */
        factor *= (seq_t)m;
        factor *= (seq_t)(m - 1);
        m -= 2;
    }

    return sum;
}
