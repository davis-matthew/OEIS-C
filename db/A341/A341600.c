/*
 * A341600  One of the two successive approximations up to 2^n for the 2-adic
 *          integer sqrt(-3/5). This is the 1 (mod 4) case.
 * https://oeis.org/A341600
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) if 5*a(n-1)^2 + 3 is divisible by 2^(n+1),
 *            otherwise a(n-1) + 2^(n-1). Initial value: a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For other indices return 0. */
    if (n < 2) return (seq_t)0;

    /* a(2) = 1 is the starting point. */
    seq_t value = (seq_t)1;

    /* Compute successive approximations up to index n. */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute 5*value^2 + 3. */
        seq_t t = value;
        t *= value;          /* value^2      */
        t *= (seq_t)5;       /* 5*value^2   */
        t += (seq_t)3;       /* 5*value^2+3 */

        /* Test divisibility by 2^(i+1).  For powers of two this is a
           simple bitmask operation. */
        seq_t two_pow = pow2(i + 1);        /* 2^(i+1)   */
        seq_t mask    = two_pow - (seq_t)1; /* 2^(i+1) - 1 */
        if ( (t & mask) != (seq_t)0 )
        {
            /* Not divisible – add 2^(i-1). */
            value += pow2(i - 1);
        }
        /* else keep current value */
    }

    return value;
}
