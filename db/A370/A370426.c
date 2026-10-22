/*
 * A370426  Number of permutations of [n] having exactly two adjacent
 *          2-cycles.
 * https://oeis.org/A370426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Sum_{k=0..floor(n/2)-2} (-1)^k * (n-k-2)! / k!
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                    /* First three terms are 0 */

    /* k_max = floor(n/2) - 2  (non‑negative for n >= 4) */
    idx_t k_max = (n / 2) - 2;

    /* term = (n-k-2)! / k!  ;  start with k = 0  */
    seq_t term = 1;
    for (idx_t i = 2; i <= n - 2; ++i)
        term *= (seq_t)i;            /* term = (n-2)!  at k = 0 */

    seq_t sum = term;                /* k = 0 contributes +term */

    for (idx_t k = 1; k <= k_max; ++k)
    {
        /* term_{k} = term_{k-1} / (k * (n-k-1))  (exact division) */
        idx_t denom = k * (n - k - 1);
        term /= (seq_t)denom;

        /* Add or subtract depending on parity of k */
        if ((k & 1) == 0)   /* even k => positive sign */
            sum += term;
        else
            sum -= term;
    }

    /* The sum is always even, so integer division by 2 is safe */
    return sum / (seq_t)2;
}
