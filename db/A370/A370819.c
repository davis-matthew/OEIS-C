/*
 * A370819  Number of subsets of {1..n-1} whose cardinality is one less than
 *          the length of the binary expansion of n; a(0) = 0.
 * https://oeis.org/A370819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n-1, floor_log2_uint64(n)) for n>0; a(0)=0
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-01)
 * Helpers:   binomial, ilog2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ilog2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    idx_t k = ilog2(n);
    return binomial(n - 1, k);
}
