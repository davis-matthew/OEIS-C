/*
 * A369724  Expansion of e.g.f. exp( (5/2) * (1-sqrt(1-4*x)) ).
 * https://oeis.org/A369724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=5; a(n) = 2*(2*n-3)*a(n-1) + 25*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = (seq_t)1;      /* a(n-2) */
    seq_t prev1 = (seq_t)5;      /* a(n-1) */
    seq_t curr  = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)2 * (seq_t)(2*i - 3);
        curr = factor * prev1 + (seq_t)25 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
