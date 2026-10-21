/*
 * A290862  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 131", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; * If n is odd: a(n) = 2^(n+1) - 1 * If n is even>0:
 *            a(n) = 2^(2^(⌊log2 n⌋−1)) - 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    if (n & 1ULL) {                       /* odd case */
        idx_t k = n + 1;
        return pow2(k) - (seq_t)1;
    }

    /* even case */
    idx_t half = n >> 1;                   /* floor(n/2) */
    /* floor(log2(half)) */
    idx_t m = 63 - __builtin_clzll((unsigned long long)half);
    idx_t k = 1ULL << m;                   /* 2^m */
    return pow2(k) - (seq_t)1;
}
