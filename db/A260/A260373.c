/*
 * A260373  The nearest perfect square to n!
 * https://oeis.org/A260373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = square of the nearest integer to sqrt(n!) (A055227(n)^2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   floor_sqrt, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

static inline seq_t compute_factorial(idx_t n)
{
    seq_t f = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        f = mul_small(f, (unsigned int)i);
    return f;
}

seq_t a(idx_t n)
{
    seq_t fact = compute_factorial(n);

    seq_t s = floor_sqrt(fact);          /* floor(sqrt(n!))          */
    seq_t sq1 = seq_mul(s, s);           /* s^2                      */
    seq_t sp1 = s + (seq_t)1;            /* s + 1                    */
    seq_mul(sp1, sp1);                  /* reuse result as sq2       */
    seq_t sq2 = seq_mul(sp1, sp1);       /* (s+1)^2                  */

    seq_t diff1 = fact - sq1;            /* fact - s^2               */
    seq_t diff2 = sq2 - fact;            /* (s+1)^2 - fact           */

    return (diff1 <= diff2) ? sq1 : sq2;
}
