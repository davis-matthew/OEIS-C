/*
 * A271102  a(n) is multiplicative with a(p^e) = -1 if e=2, a(p^e) = 0 if e=1
 *          or e>2.
 * https://oeis.org/A271102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = μ(√n) if n is a perfect square, 0 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (a(n) is 1 for n=1, -1 for n=p², 0 for all other n) */

#include <stdint.h>
#include "../utils/mobius.h"

/* Integer square root of a 64‑bit unsigned integer, using Newton's method. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t x = n;
    idx_t y = (x >> 1) + 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1; a(0) not defined */
    idx_t sq = isqrt_u64(n);
    return (sq * sq == n) ? mobius(sq) : (seq_t)0;
}
