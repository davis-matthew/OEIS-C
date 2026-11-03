/*
 * A353555  a(n) = 1 if n is an even number with an even number of prime
 *          factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A353555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even and the number of prime factors of n
 *            (counted with multiplicity) is even, otherwise 0
 * Range:     u8 n <= 1795, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 8592 (2026-11-01)
 * Helpers:   bigomega_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega_seq.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1, but handle n <= 1 gracefully */
    if (n <= 1)
        return 0;

    /* Odd numbers cannot satisfy the condition */
    if ((n & 1) != 0)
        return 0;

    /* n is even */
    seq_t factor_count = bigomega_seq((seq_t)n);

    /* Return 1 if factor_count is even, else 0 */
    return ((factor_count & 1) == 0) ? 1 : 0;
}
