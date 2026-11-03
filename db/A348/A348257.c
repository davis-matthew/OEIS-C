/*
 * A348257  Number of ways we can write [n] as the union of 2 sets of sizes i,
 *          j which intersect in exactly 2 elements (2 < i,j < n; i = j
 *          allowed).
 * https://oeis.org/A348257
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,2) * ( 2^(n-3) - 1 )
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 33 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 4. For smaller n we return 0. */
    if (n < 4) return 0;
    seq_t bin = binomial(n, 2);          /* n*(n-1)/2 */
    seq_t pow2 = ipow_u64_idx(2, n - 3);     /* 2^(n-3) */
    pow2 -= 1;                           /* 2^(n-3) - 1 */
    return bin * pow2;                   /* binomial(n,2) * (2^(n-3) - 1) */
}
