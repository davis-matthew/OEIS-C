/*
 * A327007  a(n) = number of iterations of f(x)=floor((x^2+n)/(2x)) starting
 *          at x=n to reach the value floor(sqrt(n)) (=A000196(n)).
 * https://oeis.org/A327007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of f(x)=⌊(x²+n)/(2x)⌋ starting at
 *            x=n until x reaches ⌊√n⌋. f(x) can be computed as ⌊(x+⌊n/x⌋)/2⌋,
 *            avoiding overflow
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* target value: floor(sqrt(n)) */
    idx_t s = _floor_isqrt_u64(n);
    seq_t target = (seq_t)s;

    seq_t x = (seq_t)n;           /* current value */
    seq_t cnt = 0;                /* iteration counter */

    while (x != target)
    {
        /* compute floor(n / x) safely using integer division */
        idx_t nx = n / (idx_t)x;   /* n / x is an idx_t */
        seq_t ndiv = (seq_t)nx;    /* promote to seq_t for addition */

        /* next value of the iteration */
        x = (x + ndiv) / 2;       /* floor((x + n/x)/2) */
        ++cnt;
    }
    return cnt;
}
