/*
 * A365196  a(n) is the least k such that 2^k + n is not squarefree.
 * https://oeis.org/A365196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the least k ≥ 0 such that 2^k + n is not squarefree.
   This implementation tests successive k using the is_squarefree
   and pow2 utilities. */
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t k = 0;
    for (;;) {
        seq_t val = pow2(k) + (seq_t)n;
        if (!is_squarefree(val))
            return (seq_t)k;
        ++k;
    }
}
