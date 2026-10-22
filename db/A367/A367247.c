/*
 * A367247  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 6.
 * https://oeis.org/A367247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 27*7^(n-1) - 47*6^(n-1) + 4*5^n (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle n=1 correctly: the loops below still work but are clearer with explicit values. */
    seq_t pow7 = 1;   /* 7^(n-1) */
    seq_t pow6 = 1;   /* 6^(n-1) */
    seq_t pow5 = 1;   /* 5^n   */
    idx_t i;

    /* compute 7^(n-1) */
    for (i = 1; i < n; ++i)
        pow7 *= 7;

    /* compute 6^(n-1) */
    for (i = 1; i < n; ++i)
        pow6 *= 6;

    /* compute 5^n */
    for (i = 1; i <= n; ++i)
        pow5 *= 5;

    /* to avoid a negative intermediate when using unsigned types, compute as */
    /* 27*pow7 + 4*pow5 - 47*pow6, which is guaranteed non‑negative for all n>=1 */
    return 27 * pow7 + 4 * pow5 - 47 * pow6;
}
