/*
 * A377963  Expansion of e.g.f. (1+x) * exp(x*(1+x)^2).
 * https://oeis.org/A377963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + (4*n-3)*a(n-2) + 3*(n-2)*n*a(n-3) Initial
 *            values: a(0) = 1, a(1) = 2, a(2) = 7 for n > 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;

    /* keep last three values in a sliding window */
    seq_t prev3 = (seq_t)1;   /* a(n-3)  */
    seq_t prev2 = (seq_t)2;   /* a(n-2)  */
    seq_t prev1 = (seq_t)7;   /* a(n-1)  */
    seq_t cur   = (seq_t)0;   /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        /* compute coefficients as seq_t to avoid intermediate overflow */
        seq_t c1 = ((seq_t)4 * (seq_t)i) - (seq_t)3;              /* 4*i-3   */
        seq_t c2 = (seq_t)3 * (seq_t)i * (seq_t)(i - 2);          /* 3*(i-2)*i */

        /* recurrence: a(i) = a(i-1) + c1*a(i-2) + c2*a(i-3) */
        cur = prev1 + c1 * prev2 + c2 * prev3;

        /* slide window for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
