/*
 * A396398  a(n) = r(n,n), where r(1,k)=f(k)=round(k*c), r(n,k)=f(r(n-1,k)),
 *          c=sqrt(2).
 * https://oeis.org/A396398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where r(1,k) = round(k * sqrt(2)), r(n,k) =
 *            round(r(n-1,k) * sqrt(2)), c = sqrt(2)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 21, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   floor_sqrt, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"
#include "../utils/floor_sqrt.h"

/* Perform round(x * sqrt(2)) using integer arithmetic.
   The function works for any size of seq_t. */
static seq_t round_sqrt2(seq_t x)
{
    /* calculate 2 * x * x */
    seq_t t = x * x;
    t = mul_small(t, 2);

    /* nearest integer to sqrt(t) */
    seq_t f  = floor_sqrt(t);
    seq_t sq1 = f * f;
    seq_t diff1 = t - sq1;

    seq_t f1 = f + 1;
    seq_t sq2 = f1 * f1;
    seq_t diff2 = sq2 - t;

    /* round up if nearer to f+1, otherwise keep f */
    return (diff1 < diff2) ? f : f1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* k = n */
    for (idx_t i = 0; i < n; ++i) /* apply r(n,n) n times */
    {
        x = round_sqrt2(x);
    }
    return x;
}
