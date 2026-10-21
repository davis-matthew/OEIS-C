/*
 * A276978  a(n) = (ceiling(n/2))^n.
 * https://oeis.org/A276978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (ceiling(n/2))^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    idx_t m = (n >> 1) + (n & 1u);   /* ceiling(n/2) */
    seq_t base = (seq_t)m;
    seq_t exp  = (seq_t)n;
    return big_pow(base, exp);
}
