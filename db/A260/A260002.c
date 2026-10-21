/*
 * A260002  Sudan Numbers: a(n)= f(n,n,n) where f is the Sudan function.
 * https://oeis.org/A260002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, bref
 * Formula:   a(n) = f(n,n,n) where the Sudan function f is defined by
 *            f(0,x,y) = x + y f(z,x,0) = x f(z,x,y) = f(z-1, f(z,x,y-1),
 *            f(z,x,y-1) + y) (z>0, y>0)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   f_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/f_rec.h"

seq_t a(idx_t n)
{
    return f_rec((idx_t)n, (seq_t)n, (seq_t)n);
}
