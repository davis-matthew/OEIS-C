/*
 * A337852  a(n) = (2^(n+1) + 1)^n.
 * https://oeis.org/A337852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(n+1) + 1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* (2^1+1)^0 = 1 */

    seq_t base = pow2(n + 1);          /* 2^(n+1) */
    base += (seq_t)1;                  /* 2^(n+1) + 1 */

    return big_pow(base, (seq_t)n);    /* (2^(n+1)+1)^n */
}
