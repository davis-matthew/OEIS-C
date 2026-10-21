/*
 * A292666  Rank of n*(e+1) when all the numbers j*(e-1) and k*(e+1), for
 *          j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*(e+1)/(e-1))
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdint.h>

static const long double _e_const = 2.7182818284590452353602874713527L;
static const long double _ratio = (_e_const + 1.0L) / (_e_const - 1.0L);

seq_t a(idx_t n)
{
    /* rank of n*(e+1) among all numbers of the form j*(e-1) or k*(e+1) */
    /* n >= 1 */
    unsigned long long m = (unsigned long long)n;
    unsigned long long extra = (unsigned long long)((long double)m * _ratio);
    unsigned long long result = m + extra;
    return (seq_t)result;
}
