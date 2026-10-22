/*
 * A400697  a(n) = A008966(n^2 + 3).
 * https://oeis.org/A400697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = A008966(n^2 + 3)
 * Range:     u8 n <= 19, u16 n <= 2331, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008966 is the indicator of squarefree numbers: 1 if n is squarefree, 0 otherwise. */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)n + (seq_t)3;
    return is_squarefree(m) ? (seq_t)1 : (seq_t)0;
}
