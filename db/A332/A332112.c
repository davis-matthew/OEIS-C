/*
 * A332112  a(n) = (10^(2n+1)-1)/9 + 10^n.
 * https://oeis.org/A332112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2n+1)-1)/9 + 10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^(2n+1) */
    idx_t exp1 = (n * 2) + 1;
    seq_t pow2n1 = pow10(exp1);
    /* (10^(2n+1)-1)/9 */
    seq_t rep = (pow2n1 - 1) / (seq_t)9;
    /* Compute 10^n */
    seq_t pown = pow10(n);
    /* Sum */
    return rep + pown;
}
