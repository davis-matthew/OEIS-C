/*
 * A337391  a(n) is the smallest n-digit number divisible by n^3.
 * https://oeis.org/A337391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n^3 * ceil(10^(n-1) / n^3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest n-digit number divisible by n^3. */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; calls for n == 0 are irrelevant but handled */
    if (n == 0) return 0;

    /* 10^(n-1) as a big integer */
    seq_t ten_pow = pow10(n - 1);

    /* n^3 as a big integer – cast to seq_t before each multiplication */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* q = ceil(10^(n-1) / n^3) */
    seq_t div   = ten_pow / cube;
    seq_t rem   = ten_pow % cube;
    if (rem != 0) div += 1;

    /* a(n) = div * n^3 */
    return div * cube;
}
