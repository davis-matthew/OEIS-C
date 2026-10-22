/*
 * A345257  Tiefenbruck sequence for the game of countably infinitely many
 *          hats.
 * https://oeis.org/A345257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = * if n < 8: a(n) = [-1, 1, 0, 0, 2, 1, 2, -1][n] * if n
 *            >= 8: let r = n mod 8 * if r in {1,2,3,4,5,6} a(n) = a(r) * else
 *            let p = a(floor(n/8)) * if p = -1 a(n) = -1 * else a(n) = p + 3
 * Range:     i8 n <= 85, i16 n <= 85, i32 n <= 85, i64 n <= 85, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init_values[8] = { -1, 1, 0, 0, 2, 1, 2, -1 };

seq_t a(idx_t n)
{
    if (n < 8)
        return init_values[n];

    /* compute n mod 8 and floor(n/8) */
    unsigned r = (unsigned)(n & 7ULL);
    if (r >= 1 && r <= 6)
        return a(r);               /* r is < 8, base case */

    seq_t parent = a(n >> 3);      /* floor(n/8) */
    return (parent == (seq_t)-1) ? (seq_t)-1 : parent + (seq_t)3;
}
