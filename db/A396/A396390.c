/*
 * A396390  a(n) = r(n,n), where r(1,k)=f(k)=floor(k*c), r(n,k)=f(r(n-1,k)),
 *          c=golden ratio.
 * https://oeis.org/A396390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where r(1,k) = floor(k * (1+sqrt(5))/2) r(n,k) =
 *            r(1, r(n-1,k)) (strict recurrence) a(1) = 1, a(2) = 4, a(3) = 9,
 *            … (first terms below)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t f(seq_t k)
{
    /* Compute floor(k * (1+sqrt(5))/2) without overflow.
       Using formula: floor(k*phi) = (k + floor(k*sqrt(5))) / 2.
       floor(k*sqrt(5)) = floor(sqrt(5*k*k)).
    */
    seq_t prod = k * k;          /* k*k   */
    prod = prod * (seq_t)5;      /* 5*k*k */
    seq_t sq = floor_sqrt(prod); /* floor(sqrt(5*k*k)) */
    return (k + sq) / (seq_t)2;
}

seq_t a(idx_t n)
{
    /* Compute r(n,n) by iterating f() n times starting from k = n. */
    seq_t r = (seq_t)n;
    for (idx_t i = 0; i < n; ++i)
        r = f(r);
    return r;
}
