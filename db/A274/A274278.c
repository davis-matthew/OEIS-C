/*
 * A274278  a(n) = ((n+1)^(n-1) - (n-1)^(n-1))/2 for n>=1.
 * https://oeis.org/A274278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^(n-1) - (n-1)^(n-1))/2 for n>=1, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 23,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;               /* a(0) = 1 */
    if (n == 1u)
        return (seq_t)0;               /* a(1) = 0 */

    seq_t exp = (seq_t)(n - 1u);         /* exponent (n-1) */
    seq_t base1 = (seq_t)(n + 1u);       /* base (n+1) */
    seq_t base2 = (seq_t)(n - 1u);       /* base (n-1) */

    seq_t up1 = big_pow(base1, exp);     /* (n+1)^(n-1) */
    seq_t up2 = big_pow(base2, exp);     /* (n-1)^(n-1) */

    /* The result fits into seq_t as required by the problem statement. */
    seq_t diff = up1 - up2;             /* numerator */
    return diff / 2;                    /* final value */
}
