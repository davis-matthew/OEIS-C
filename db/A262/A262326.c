/*
 * A262326  Number of (n+1) X (2+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1) + 2 * 3^floor((n-1)/2) (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the sequence definition */
    seq_t power1 = pow3((idx_t)n - 1);          /* 3^(n-1) */
    seq_t power2 = pow3(((idx_t)n - 1) / 2);    /* 3^floor((n-1)/2) */
    seq_t double_power2 = power2 + power2;       /* 2 * power2 */

    return power1 + double_power2;              /* a(n) */
}
