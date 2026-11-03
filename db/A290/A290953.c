/*
 * A290953  The number of permutations in S_n for which the number of reduced
 *          words is maximized with respect to the numbers of braid and
 *          commutation classes: |R(w)| = |B(w)| * |C(w)|.
 * https://oeis.org/A290953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = catalan(n) + n - 2 for n > 1; a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;
    return catalan(n) + (seq_t)n - 2;
}
