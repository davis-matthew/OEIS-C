/*
 * A274230  Number of holes in a sheet of paper when you fold it n times and
 *          cut off the four corners.
 * https://oeis.org/A274230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (2^ceil(n/2)-1) * (2^floor(n/2)-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 962 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) = (n + 1) / 2   (integer division, unsigned) */
    /* floor(n/2) = n / 2        (integer division, unsigned) */
    idx_t r = (n + 1) / 2;          /* number of folds along one axis */
    idx_t s = n / 2;                /* number of folds along the other axis */

    seq_t u = ipow((seq_t)2, (seq_t)r) - (seq_t)1;   /* 2^r - 1   */
    seq_t v = ipow((seq_t)2, (seq_t)s) - (seq_t)1;   /* 2^s - 1   */

    return u * v;
}
