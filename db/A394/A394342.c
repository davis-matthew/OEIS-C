/*
 * A394342  a(n) = n*(3^n - 1) + 1.
 * https://oeis.org/A394342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3^n - 1) + 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t pow3 = big_pow((seq_t)3, sn);
    seq_t res  = sn * (pow3 - (seq_t)1) + (seq_t)1;
    return res;
}
