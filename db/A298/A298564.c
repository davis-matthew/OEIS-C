/*
 * A298564  a(n) = (3^(n+2)+11)/2 - 5*2^(n+1) + 2*n.
 * https://oeis.org/A298564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(n+2)+11)/2 - 5*2^(n+1) + 2*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t term1 = (pow3(n + 2) + (seq_t)11) / (seq_t)2;
    seq_t term2 = pow2(n + 1) * (seq_t)5;
    seq_t term3 = ((seq_t)n * (seq_t)2);
    return term1 - term2 + term3;
}
