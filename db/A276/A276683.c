/*
 * A276683  Number of divisors of the n-th 10-gonal number.
 * https://oeis.org/A276683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d( n * (4*n - 3) ) * where d(m) is the number of positive
 *            divisors of m. * The n-th 10-gonal number P10(n) equals
 *            n*(4*n-3), * so a(n) is the divisor count of that number
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Compute the number of divisors of the n-th 10‑gonal number. */
seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the problem statement. */
    uint64_t n64 = (uint64_t)n;                /* promote to 64‑bit for arithmetic */
    uint64_t p   = n64 * (4 * n64 - 3);        /* P10(n) = n*(4n-3); fits in 64‑bit for n <= 10000 */

    return (seq_t)divisor_count_u64(p);
}
