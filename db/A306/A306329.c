/*
 * A306329  If n = Product (p_j^k_j) then a(n) = Product (p_j)^Sum (k_j).
 * https://oeis.org/A306329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n)^bigomega(n)
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow, bigomega, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"
#include "../utils/bigomega.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t r = rad(n);
    seq_t w = bigomega(n);
    return big_pow(r, w);
}
