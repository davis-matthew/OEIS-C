/*
 * A394522  E.g.f. A(x) satisfies A(x) = 1/(1 - 4*x*A(x)^2)^(1/4).
 * https://oeis.org/A394522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Π_{k=1..n} (6n + 5 – 4k) / (6n + 1) which simplifies to:
 *            a(0) = 1 a(n) = Π_{k=2..n} (6n + 5 – 4k) for n >= 1 (since the k
 *            = 1 term cancels the denominator)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = 1;
    seq_t base = (seq_t)(6 * n + 5);   /* 6*n + 5 as seq_t */

    /* Multiply the remaining terms; k = 1 term cancels (6n+1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t term = base - (seq_t)4 * k;
        res *= term;
    }
    return res;
}
