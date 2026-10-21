/*
 * A267226  Number of length-n 0..2 arrays with no following elements greater
 *          than or equal to the first repeated value.
 * https://oeis.org/A267226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + n*2^(n-2) - 1 for n > 1 a(1) = 3
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;

    seq_t power_high = pow2(n + 1);            /* 2^(n+1) */
    seq_t power_mid  = pow2(n - 2);            /* 2^(n-2) */
    seq_t term_middle = (seq_t)n * power_mid;  /* n * 2^(n-2) */

    return power_high + term_middle - (seq_t)1;
}
