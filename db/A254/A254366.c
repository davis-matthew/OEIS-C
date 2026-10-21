/*
 * A254366  a(n) = 4^n + 10*2^n + 4*3^n + 20.
 * https://oeis.org/A254366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 10*2^n + 4*3^n + 20
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   mul_small, pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t result = pow4(n);                                /* 4^n */
    result += mul_small(pow2(n), (unsigned)10);           /* +10*2^n */
    result += mul_small(pow3(n), (unsigned)4);            /* +4*3^n */
    result += (seq_t)20;                                  /* +20 */
    return result;
}
