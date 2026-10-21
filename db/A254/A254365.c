/*
 * A254365  a(n) = 2^(n+2) + 3^n + 10.
 * https://oeis.org/A254365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+2) + 3^n + 10
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t term2 = pow2(n + 2); /* 2^(n+2) */
    seq_t term3 = pow3(n);     /* 3^n     */
    return term2 + term3 + (seq_t)10;
}
