/*
 * A270439  Alternating sum of nonsquares (A000037).
 * https://oeis.org/A270439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = if n even: -n/2 if n odd (n = 2k-1): k + floor(1/2 +
 *            sqrt(2k)) where k = (n+1)/2 (equivalent to a(2k-1) = k +
 *            floor(0.5 + sqrt(2k)), a(2k) = -k)
 * Range:     i8 n <= 62, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Even index */
    if ((n & 1ULL) == 0) {
        idx_t k = n >> 1;            /* n = 2k */
        return -(seq_t)k;
    }

    /* Odd index */
    idx_t k = (n + 1) >> 1;          /* n = 2k-1  ->  k = (n+1)/2 */
    seq_t K   = (seq_t)k;
    seq_t twok = K << 1;                   /* 2k */
    seq_t t   = floor_sqrt(twok);          /* floor(sqrt(2k)) */

    /* inc = 1 if floor(sqrt(2k)+0.5) = t + 1 , else 0.
       Condition: 8k >= (2t+1)^2    => (twok<<2) >= ((t<<1)+1)^2   */
    seq_t inc = ((twok << 2) >= ((t << 1) + 1) * ((t << 1) + 1)) ? 1 : 0;

    /* a(2k-1) = k + t + inc */
    return K + t + inc;
}
