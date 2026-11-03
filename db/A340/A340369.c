/*
 * A340369  a(n) = 1 if n has at most 3 prime factors when counted with
 *          multiplicity, 0 otherwise.
 * https://oeis.org/A340369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A001222(n) <= 3]
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9985 (2026-11-01)
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    return (bigomega(n) <= 3) ? 1 : 0;
}
