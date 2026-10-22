/*
 * A354329  Triangular number nearest to the sum of the first n positive
 *          triangular numbers.
 * https://oeis.org/A354329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (t^2 + t) / 2, where t = floor(sqrt(n * (n + 1) * (n + 2)
 *            / 3))
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t and compute the needed value safely */
    seq_t N = (seq_t)n;
    seq_t num = N * (N + 1) * (N + 2);
    seq_t div = num / (seq_t)3;          /* integer division */
    seq_t t   = floor_sqrt(div);         /* floor of the square root */
    seq_t res = (t * t + t) / (seq_t)2;  /* (t^2 + t) / 2 */
    return res;
}
