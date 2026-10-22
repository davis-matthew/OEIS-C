/*
 * A352681  a(n) = [x^n] (3*x + 1)*(1 - sqrt(1 - 4*x))/(2*x).
 * https://oeis.org/A352681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A352680(4, n) a(0) = 1, a(1) = 4 Recurrence for n ≥ 2:
 *            a(n) = a(n-1) * ((14*n + 2)*(2*n - 3)) / ((7*n - 6)*(n + 1))
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 13, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t prev1 = (seq_t)4;   /* a(1) */
    seq_t prev2 = (seq_t)1;   /* a(0) */

    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t num = (seq_t)(14*i + 2) * (seq_t)(2*i - 3);
        seq_t den = (seq_t)(7*i - 6) * (seq_t)(i + 1);
        cur = (prev1 * num) / den;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
