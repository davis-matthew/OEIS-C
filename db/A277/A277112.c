/*
 * A277112  a(n) = floor(n*(1+sqrt(5))/Pi).
 * https://oeis.org/A277112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(1+sqrt(5))/Pi)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* high‑precision constant (80‑bit long double) */
    static const long double alpha = 1.0300724295689167L;
    /* compute using floating arithmetic, cast to native unsigned long long first,
       then convert to seq_t – this is valid for any seq_t that can be
       constructed from an unsigned long long. */
    unsigned long long val = (unsigned long long)((long double)n * alpha);
    return (seq_t)val;
}
