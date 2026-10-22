/*
 * A350221  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor((n/k)^2).
 * https://oeis.org/A350221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (-1)^(k+1) * ⌊(n/k)^2⌋
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard for n = 0 (offset starts at 1) */
    if (n == 0)
        return 0;

    seq_t result = 0;
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2, used for all divisions */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k * (seq_t)k;        /* k^2 */
        seq_t sq = nn / kk;                   /* floor((n/k)^2) = floor(n^2/k^2) */
        if (k & 1U)                           /* k odd → add */
            result += sq;
        else                                  /* k even → subtract */
            result -= sq;
    }
    return result;
}
