/*
 * A264965  Permutation of nonnegative integers: a(n) = A263273(A057889(n)).
 * https://oeis.org/A264965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A263273(A057889(n))
 * Range:     u8 n <= 158, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   adj_rev_base2, adj_rev_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the adjusted reverse in base 2 followed by the adjusted reverse in base 3. */

#include <stdint.h>
#include "../utils/adj_rev_base2.h"
#include "../utils/adj_rev_base3.h"

/* Sequence function */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t b2 = adj_rev_base2(sn);
    return adj_rev_base3(b2);
}
