/*
 * A365152  G.f. satisfies A(x) = ( 1 + x*A(x)^2 / (1 - x*A(x))^3 )^3.
 * https://oeis.org/A365152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(3*(n+k+1),k) *
 *            binomial(n+2*k-1,n-k)/(n+k+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t tmp = n + k + 1;                 /* n + k + 1  */
        idx_t arg1 = 3 * tmp;                  /* 3*(n + k + 1) */
        seq_t part1 = binomial(arg1, k);

        idx_t arg2_top = n + 2 * k - 1;         /* n + 2k - 1 */
        idx_t arg2_bot = n - k;                /* n - k */
        seq_t part2 = binomial(arg2_top, arg2_bot);

        seq_t prod = part1 * part2;
        seq_t div  = (seq_t)tmp;               /* n + k + 1 */
        seq_t term = prod / div;               /* exact division */
        sum += term;
    }
    return sum;
}
