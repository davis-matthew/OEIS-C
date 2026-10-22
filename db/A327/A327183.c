/*
 * A327183  a(n) = [(2n+1)e] - [(n+1)e] - [ne], where [ ] = floor.
 * https://oeis.org/A327183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2n+1)*e) - floor((n+1)*e) - floor(n*e)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t P = 517656ULL;   /* numerator of a convergent of e */
static const uint64_t Q = 190435ULL;   /* denominator  > 2*max_n */

/* floor(k * e) computed exactly by using the rational P/Q  (error < 1/(k+1)) */
static inline seq_t floor_mul(idx_t k)
{
    return (seq_t)(( (uint64_t)k * P ) / Q);
}

seq_t a(idx_t n)
{
    seq_t f2 = floor_mul(2*n + 1);   /* floor((2n+1)*e)  */
    seq_t f1 = floor_mul(n + 1);     /* floor((n+1)*e)   */
    seq_t f0 = floor_mul(n);         /* floor(n*e)       */
    int64_t val = (int64_t)f2 - (int64_t)f1 - (int64_t)f0;
    return (seq_t)val;               /* 0 or 1 for all n  */
}
