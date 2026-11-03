/*
 * A295904  Number of (not necessarily maximal) cliques in the n-sun graph.
 * https://oeis.org/A295904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 4*n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);          /* 2^n  */
    val += 4 * (seq_t)n;          /* + 4*n */
    return val;
}
