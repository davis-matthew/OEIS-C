/*
 * A294371  Lexicographically earliest sequence of distinct positive numbers
 *          such that, for any n > 0, a(3*n) = 2*a(n).
 * https://oeis.org/A294371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n - floor(n/3)) - 1 if n%3≠0; a(n) = 2*a(n/3) if
 *            n%3==0
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
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
    /* n is 1‑based. Return 0 for an illegal 0 index. */
    if (n == 0) return 0;

    idx_t m = n;
    idx_t k = 0;
    while ((m % 3) == 0) {
        m /= 3;
        k++;
    }

    seq_t base = 2 * ((seq_t)m - (seq_t)(m / 3)) - 1;
    if (k == 0) return base;

    seq_t multiplier = pow2(k);
    return multiplier * base;
}
