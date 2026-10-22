/*
 * A341249  a(n) = floor(r*floor(s*n)), where r = 2 + sqrt(2) and s = sqrt(2).
 * https://oeis.org/A341249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2+sqrt(2))*floor(sqrt(2)*n))
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   floor_sqrt, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;                       /* n as seq_t */
    seq_t sq   = seq_mul(sn, sn);                /* n^2          */
    seq_t dqs  = mul_small(sq, 2);               /* 2*n^2        */
    seq_t m    = floor_sqrt(dqs);                /* floor(sqrt(2)*n) */

    seq_t sqm  = seq_mul(m, m);                  /* m^2          */
    seq_t dqs2 = mul_small(sqm, 2);              /* 2*m^2        */
    seq_t f2   = floor_sqrt(dqs2);               /* floor(sqrt(2)*m) */

    return m + m + f2;                           /* 2*m + f2      */
}
