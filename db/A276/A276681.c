/*
 * A276681  Number of divisors of the n-th octagonal number.
 * https://oeis.org/A276681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005( A000567(n) ) = number of divisors of n*(3*n-2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where A000567(n) = n*(3*n-2) is the n-th octagonal number. */

#include <stdint.h>                     /* for uint64_t */
#include "../utils/divisor_count_u64.h"  /* provides divisor_count_u64 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0
       to avoid undefined behaviour of divisor_count_u64(0). */
    if (n == 0)
        return 0;

    /* Compute the n-th octagonal number in 64‑bit arithmetic.  Since
       n will typically be small enough to keep the product within
       uint64_t limits, this is safe for the test ranges. */
    uint64_t un   = (uint64_t)n;
    uint64_t oct = un * (3ULL * un - 2ULL);

    /* Return the divisor count as a seq_t value. */
    return divisor_count_u64(oct);
}
