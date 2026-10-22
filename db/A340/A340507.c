/*
 * A340507  a(n) = floor(sqrt(2*n)) - A003056(n).
 * https://oeis.org/A340507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2*n)) - floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 7, u16 n <= 1549, u32 n <= 8657, u64 n <= 8657,
 *            u128 n <= 8657, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                    /* convert index to seq_t type */
    seq_t s1 = floor_sqrt(tn * 2);          /* floor(sqrt(2*n))            */
    seq_t t  = floor_sqrt(tn * 8 + 1);      /* floor(sqrt(8*n+1))          */
    seq_t idx = (t - 1) / 2;                /* floor((sqrt(8*n+1)-1)/2)   */
    return s1 - idx;
}
