/*
 * A302583  a(n) = ((n + 1)^n - (n - 1)^n)/2.
 * https://oeis.org/A302583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n + 1)^n - (n - 1)^n)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t exp  = (seq_t)n;
    seq_t pow1 = big_pow((seq_t)(n + 1), exp); /* (n+1)^n */
    seq_t pow2 = big_pow((seq_t)(n - 1), exp); /* (n-1)^n */

    return (pow1 - pow2) / (seq_t)2;
}
