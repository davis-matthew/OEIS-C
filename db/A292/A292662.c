/*
 * A292662  Rank of n*sqrt(5) when all the numbers h*sqrt(2), j*sqrt(3),
 *          k*sqrt(5), for h>=1, j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*sqrt(5/2)) + floor(n*sqrt(5/3)) Equivalent
 *            forms: - floor(n*sqrt(5/2)) = floor( sqrt(10)*n ) / 2 -
 *            floor(n*sqrt(5/3)) = floor( sqrt(15)*n ) / 3 This implementation
 *            uses integer arithmetic only, employing floor_sqrt to compute
 *            the integer square root of needed values
 * Range:     u8 n <= 2, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, return 0 for safety */

    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Compute n^2 as seq_t */
    seq_t nsq = sn * sn;

    /* Compute floor(sqrt(10 * n^2)) and floor(sqrt(15 * n^2)) */
    seq_t t1 = floor_sqrt( (seq_t)10 * nsq );   /* floor(sqrt(10*n^2)) */
    seq_t t2 = floor_sqrt( (seq_t)15 * nsq );   /* floor(sqrt(15*n^2)) */

    /* Obtain floor(n*sqrt(5/2)) and floor(n*sqrt(5/3)) by division */
    seq_t f1 = t1 / 2;   /* floor( floor(sqrt(10*n^2)) / 2 ) = floor(n*sqrt(5/2)) */
    seq_t f2 = t2 / 3;   /* floor( floor(sqrt(15*n^2)) / 3 ) = floor(n*sqrt(5/3)) */

    /* Final rank */
    seq_t res = sn + f1 + f2;

    return res;
}
