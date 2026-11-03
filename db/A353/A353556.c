/*
 * A353556  a(n) = 1 if n is an even number with an odd number of prime
 *          factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A353556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even and its number of prime factors (counted
 *            with multiplicity) is odd, otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9406 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Check for evenness */
    if ((n & 1ULL) == 0) {
        /* bigomega returns the total count of prime factors (with multiplicity) */
        seq_t omega = bigomega(n);
        /* Return 1 for odd omega, 0 otherwise */
        return (omega & 1) ? 1 : 0;
    }
    /* For odd n the condition fails */
    return 0;
}
