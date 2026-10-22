/*
 * A372683  Least squarefree number >= 2^n.
 * https://oeis.org/A372683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least squarefree number >= 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_squarefree, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t target = pow2(n);

    if (is_squarefree(target))
        return target;

    seq_t inc = 1;
    for (;; ++inc) {
        seq_t cand = target + inc;
        if (is_squarefree(cand))
            return cand;
    }
}
