/*
 * A359377  a(n) = 1 if 3*n is squarefree, otherwise 0.
 * https://oeis.org/A359377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if 3*n is squarefree, 0 otherwise
 * Range:     u8 n <= 86, u16 n <= 9910, u32 n <= 9910, u64 n <= 9910,
 *            u128 n <= 9910, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* 3*n fits into seq_t but n is idx_t; cast n to seq_t first */
    seq_t m = (seq_t)n * 3;
    return is_squarefree(m) ? (seq_t)1 : (seq_t)0;
}
