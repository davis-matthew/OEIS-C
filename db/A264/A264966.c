/*
 * A264966  Permutation of nonnegative integers: a(n) = A057889(A263273(n)).
 * https://oeis.org/A264966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A057889(A263273(n))
 * Range:     u8 n <= 244, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   adjusted_reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) as the composition of an adjusted reverse in base 3
   followed by an adjusted reverse in base 2. */

#include <stdbool.h>
#include "../utils/adjusted_reverse.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = adjusted_reverse(x, 3); /* A263273: base‑3 adjusted reverse */
    x = adjusted_reverse(x, 2); /* A057889: base‑2 adjusted reverse */
    return x;
}
