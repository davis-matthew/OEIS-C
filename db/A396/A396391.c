/*
 * A396391  a(n) = r(n,n), where r(1,k)=f(k)=ceiling(k*c), r(n,k)=f(r(n-1,k)),
 *          c=golden ratio.
 * https://oeis.org/A396391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = r(n,n) where * r(1,k) = ceil(k*φ) (φ = (1+√5)/2), *
 *            r(n,k) = r(n-1, r(n-1,k)) for n>1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"        /* provides floor_sqrt(seq_t) */

/* Computes f(x) = ceil(x*φ) for integer x.
   Because φ is irrational, ceil(x*φ) = floor(x*φ) + 1. */
static inline seq_t f(seq_t x)
{
    /* floor(x*√5) = floor_sqrt(5*x*x). */
    seq_t sq = floor_sqrt(5 * x * x);
    /* floor(x*φ) = floor((x + floor(x*√5)) / 2). */
    seq_t floor_phi = (x + sq) >> 1;     /* integer division by 2 */
    return floor_phi + 1;
}

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;                /* start with k = n */
    for (idx_t i = 0; i < n; ++i)
        val = f(val);                    /* apply f iteratively n times */
    return val;
}
