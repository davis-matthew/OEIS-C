/*
 * A329839  Beatty sequence for (-1+sqrt(41))/4.
 * https://oeis.org/A329839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * ((sqrt(41) - 1) / 4) )
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert n to seq_t for arithmetic */
    seq_t nn = (seq_t)n;

    /* Compute floor( n * sqrt(41) ) as floor_sqrt(41 * n^2) */
    seq_t tmp = nn * nn;                 /* n^2 */
    tmp = tmp * (seq_t)41;               /* 41 * n^2 */
    seq_t s = floor_sqrt(tmp);           /* floor(n * sqrt(41)) */

    /* a(n) = floor( (s - n) / 4 ) */
    seq_t diff   = s - nn;
    seq_t result = diff / (seq_t)4;

    return result;
}
