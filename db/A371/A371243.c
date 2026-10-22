/*
 * A371243  Number of factorizations of n into factors > 2.
 * https://oeis.org/A371243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of factorizations of n into factors > 2 a(n) counts
 *            unordered factorizations of n into integers > 2, including the
 *            trivial factorization n itself. The implementation uses a simple
 *            recursive search with a minimal non‑decreasing ordering
 *            constraint to avoid duplicate permutations
 * Range:     u8 n <= 8639, u16 n <= 9612, u32 n <= 9612, u64 n <= 9612,
 *            u128 n <= 9612, big any n
 * Helpers:   count_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_factorization.h"

seq_t a(idx_t n)
{
    /* OEIS offset : a(1)=1, a(2)=0 */
    switch (n) {
    case 1: return 1;
    case 2: return 0;
    default: return count_factorization(n, 3);
    }
}
