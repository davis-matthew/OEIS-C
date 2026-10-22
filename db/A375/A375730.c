/*
 * A375730  a(n) = n^n + n*n + n+n + 1.
 * https://oeis.org/A375730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n + (n+1)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t nn   = big_pow((seq_t)n, (seq_t)n);
    seq_t np1  = (seq_t)(n + 1);
    seq_t np1sq = np1 * np1;
    return nn + np1sq;
}
