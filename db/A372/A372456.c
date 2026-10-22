/*
 * A372456  a(n) = 1 if abs(tan(n)) > 1, 0 otherwise.
 * https://oeis.org/A372456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if abs(tan(n)) > 1, 0 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    long double t = tanl((long double)n);
    return (seq_t)(fabsl(t) > 1.0L);
}
