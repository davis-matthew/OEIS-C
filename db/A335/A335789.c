/*
 * A335789  a(n) = time to the nearest second at the n-th instant (n>=0) when
 *          the hour and minute hands on a clock face coincide, starting at
 *          time 0:00.
 * https://oeis.org/A335789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*43200/11)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is implemented using integer arithmetic: a(n) = (86400*n + 11) / 22 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Promote n to seq_t before multiplication to avoid overflow on the native type */
    seq_t prod = (seq_t)n * (seq_t)86400;
    return (prod + (seq_t)11) / (seq_t)22;
}
