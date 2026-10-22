/*
 * A307118  a(1) = 0; for n>1, a(n) = dr(n-1) + dr(n) + dr(n+1), where dr(n)
 *          is the number of nontrivial divisors of n (A070824).
 * https://oeis.org/A307118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = dr(n-1) + dr(n) + dr(n+1)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   dr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* dr(n) = number of non‑trivial divisors of n (A070824)   */

#include <stdint.h>
#include "../utils/dr.h"

/* Sequence a(n).  External linkage is required by the build. */
seq_t
a(idx_t n)
{
    if (n == 1)
        return 0;
    return dr(n - 1) + dr(n) + dr(n + 1);
}
