/*
 * A360778  Smallest number k such that n + k is a refactorable number.
 * https://oeis.org/A360778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{k ≥ 0 | n + k is refactorable}
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   is_refactorable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_refactorable.h"

/* Smallest k such that n + k is a refactorable number.
   For n = 0, a(0) = 1 by definition. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t x = (seq_t)n;
    seq_t k = 0;

    while (!is_refactorable(x)) {
        ++x;
        ++k;
    }
    return k;
}
