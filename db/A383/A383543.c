/*
 * A383543  a(n) is the largest number k such that i*n + 1 is squarefree for
 *          all 0 <= i <= k.
 * https://oeis.org/A383543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A383543(n): the largest k such that i*n+1 is squarefree for all
   0 <= i <= k. One must check each successive value of i until a non‑squarefree
   integer appears. */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t i = 0;
    for (;; ++i) {
        seq_t val = (seq_t)((i * n) + 1);
        if (!is_squarefree(val))
            break;
    }
    /* After the loop i is the first index with non‑squarefree value, so a(n)=i-1 */
    return (seq_t)(i - 1);
}
