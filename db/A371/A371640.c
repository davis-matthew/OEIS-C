/*
 * A371640  a(n) = 3^(2*n + valuation(n, 3)) = 3^A371638(n).
 * https://oeis.org/A371640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(2*n + valuation(n,3))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   _valuation_of_3, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/_valuation_of_3.h"

seq_t a(idx_t n)
{
    /* n >= 1 by OEIS specification */
    idx_t v = _valuation_of_3(n);
    idx_t exp = (n << 1) + v;        /* 2*n + v_3(n) */
    return big_pow((seq_t)3, (seq_t)exp);
}
