/*
 * A286922  {0->01}-transform of the Sturmian word A080764.
 * https://oeis.org/A286922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*r) – floor((n+2)*r) + 1, where r = (3 -
 *            sqrt(2))/7. The implementation uses only integer arithmetic; no
 *            floating‑point routines are required
 * Range:     u8 n <= 3, u16 n <= 86, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* Compute floor(k*sqrt(2)) exactly via floor_sqrt(2*k^2). */
static inline seq_t floor_sqrt2_times_k(seq_t k)
{
    seq_t two = (seq_t)2;
    seq_t val = two * k * k;          /* val = 2*k*k   */
    return floor_sqrt(val);           /* floor(sqrt(val)) = floor(k*sqrt(2)) */
}

/* Compute floor(k*r) where r = (3 - sqrt(2))/7.
   Using the identity:
       floor(k*r) = floor( (3k - floor(k*sqrt(2))) / 7 )
   and correcting for the case when the numerator is exactly divisible
   by 7.                                          */
static inline seq_t floor_r_k(seq_t k)
{
    seq_t s   = floor_sqrt2_times_k(k);
    seq_t ub  = (seq_t)3 * k - s;          /* upper bound of numerator */
    seq_t rem = ub % (seq_t)7;
    seq_t div = ub / (seq_t)7;
    if (rem == (seq_t)0) {
        return div - (seq_t)1;
    } else {
        return div;
    }
}

/* External function required by the build.                     */
seq_t a(idx_t n)
{
    seq_t k1 = (seq_t)n + (seq_t)1;
    seq_t k2 = (seq_t)n + (seq_t)2;

    seq_t f1 = floor_r_k(k1);
    seq_t f2 = floor_r_k(k2);

    /* a(n) = 1 if floor((n+1)*r) == floor((n+2)*r), otherwise 0. */
    return (f2 == f1) ? (seq_t)1 : (seq_t)0;
}
