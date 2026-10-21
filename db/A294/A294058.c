/*
 * A294058  Number of set partitions of [n] such that the maximal absolute
 *          difference between consecutive elements within a block equals
 *          nine.
 * https://oeis.org/A294058
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A287281(n) - A287280(n)
 * Range:     u8 none, u16 n <= 11, u32 n <= 16, u64 n <= 28, u128 n <= 51,
 *            big any n
 * Helpers:   partitions_leq_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A287281(n) = Number of set partitions of [n] whose maximal
 * consecutive difference ≤ 9.
 * A287280(n) = same with bound ≤ 8. */

#include <stdlib.h>
#include <string.h>
#include "../utils/partitions_leq_k.h"

seq_t a(idx_t n)
{
    seq_t a9 = partitions_leq_k(n, 9);
    seq_t a8 = partitions_leq_k(n, 8);
    return a9 - a8;
}
