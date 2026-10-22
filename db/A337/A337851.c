/*
 * A337851  a(n) = (2^n + 2)^n.
 * https://oeis.org/A337851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n + 2)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute base = 2^n + 2 */
    seq_t pow2n = pow2(n);
    seq_t base = pow2n + (seq_t)2;

    /* Compute (2^n + 2)^n */
    return seq_pow(base, n);
}
