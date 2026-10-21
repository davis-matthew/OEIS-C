/*
 * A292660  Rank of n*sqrt(2) when all the numbers h*sqrt(2), j*sqrt(3),
 *          k*sqrt(5), for h>=1, j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n*√(2/3)⌋ + ⌊n*√(2/5)⌋ Implemented with integer
 *            arithmetic: n*√(2/3) = n*√(6)/3, n*√(2/5) = n*√(10)/5
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* floor( n*√6 )  = floor( √(N*N*6) ) */
    seq_t s6 = floor_sqrt( N * N * (seq_t)6 );
    /* floor( n*√10)  = floor( √(N*N*10) ) */
    seq_t s10 = floor_sqrt( N * N * (seq_t)10 );

    /* a(n) = n + floor(s6/3) + floor(s10/5) */
    return N + s6 / (seq_t)3 + s10 / (seq_t)5;
}
