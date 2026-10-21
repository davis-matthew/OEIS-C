/*
 * A261393  Additive terms of the rational Collatz tree.
 * https://oeis.org/A261393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, * a(2*n) = 2*a(n), * a(2*n+1) = a(n) + 3^A000120(n)
 *            (A000120(n) = number of 1‑bits in n)
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow3_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3_local.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    if ((n & (idx_t)1) == 0)         /* even index: n = 2*m */
        return a(n >> 1) * (seq_t)2;
    else                              /* odd index:  n = 2*m+1 */
        return a(n >> 1) + pow3_local((idx_t)__builtin_popcountll(n >> 1));
}
