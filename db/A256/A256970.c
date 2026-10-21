/*
 * A256970  Smallest prime divisor of 4*n^2+1.
 * https://oeis.org/A256970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime divisor of 4*n^2+1
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 4 * nn * nn + 1;      /* 4*n^2 + 1, always odd for n ≥ 1 */

    /* Iterate over odd candidates starting at 3 */
    for (seq_t p = 3; ; p += 2)
    {
        if (p > val / p)            /* p > sqrt(val) */
            break;

        if (val % p == 0)
            return p;               /* found the smallest prime divisor */
    }

    /* If no divisor found, val itself is prime */
    return val;
}
