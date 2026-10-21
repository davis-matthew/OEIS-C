/*
 * A292659  Rank of n*sin(1) when all the numbers h, j*cos(1), k*sin(1), for
 *          h>=1, j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n*sin(1)) + floor(n*tan(1))
 * Range:     u8 n <= 75, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sinl, cosl */

seq_t a(idx_t n)
{
    /* n is at least 1; cast to long double for accurate multiplication */
    long double nl = (long double)n;

    /* Compute sin(1) and tan(1) once with high precision */
    static const long double SIN1  = sinl(1.0L);                         /* ~0.8414709848078965 */
    static const long double COS1  = cosl(1.0L);                         /* ~0.5403023058681397 */
    static const long double TAN1  = SIN1 / COS1;                       /* ~1.557407724654902 */

    /* floor(n*sin(1)) – truncation of the positive long double */
    unsigned long long fsin = (unsigned long long)(nl * SIN1);

    /* floor(n*tan(1)) */
    unsigned long long ftan = (unsigned long long)(nl * TAN1);

    /* The rank formula */
    unsigned long long result = (unsigned long long)n + fsin + ftan;

    /* Convert to the generic seq_t type (may be uint64_t or larger) */
    return (seq_t)result;
}
