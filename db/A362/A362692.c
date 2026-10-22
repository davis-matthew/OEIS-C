/*
 * A362692  Length of the "integer part" of the phi-expansion of n.
 * https://oeis.org/A362692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(log_phi(n)) for n >= 2, a(0) = 1, a(1) = 1. This is
 *            equivalent to the smallest integer m such that phi^m >= n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    /* golden ratio */
    static const double phi = (1.0 + sqrt(5.0)) / 2.0;

    double power = phi;      /* phi^1 */
    idx_t k = 1;             /* current exponent */

    /* find the least k with phi^k >= n */
    while (power < (double)n) {
        ++k;
        power *= phi;
    }

    return (seq_t)k;
}
