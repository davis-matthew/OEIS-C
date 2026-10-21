/*
 * A277690  Smallest possible number of sides of a regular polygon with unit
 *          sides and circumradius at least n.
 * https://oeis.org/A277690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( Pi / arcsin(1/(2n)) ) For n = 0 : a(0) = 3 For n =
 *            1 : a(1) = 6 For n ≥ 2 : a(n) = ceil( 2π n )
 * Range:     u8 n <= 40, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t PI_NUM = 3141592653589793ULL;    /* π ≈ 3.141592653589793  (scaled by 10^15) */
static const uint64_t PI_DEN = 1000000000000000ULL;   /* 10^15 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)6;

    /* Compute floor( 2π n ) using integer arithmetic. */
    __int128 prod = (__int128)2 * ( __int128)n * PI_NUM;
    seq_t floor_val = (seq_t)(prod / PI_DEN);

    /* Since 2πn is never an integer, ceil = floor + 1. */
    return floor_val + (seq_t)1;
}
