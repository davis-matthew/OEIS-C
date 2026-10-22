/*
 * A368131  a(n) = floor(n * log(4/3) / log(3/2)).
 * https://oeis.org/A368131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * log(4/3) / log(3/2) )
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the constant once */
    static const double alpha = log(4.0 / 3.0) / log(3.0 / 2.0);
    /* Compute the product in double precision */
    double prod = (double)n * alpha;
    /* Floor the real value and cast through an unsigned integral type */
    unsigned long long val = (unsigned long long)floor(prod + 1e-12L);
    return (seq_t)val;
}
