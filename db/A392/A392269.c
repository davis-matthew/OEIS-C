/*
 * A392269  a(n) = Sum_{k=0..floor(2*n/3)} binomial(k+2,2) *
 *          binomial(k,2*n-3*k).
 * https://oeis.org/A392269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(2*n/3)} binomial(k+2,2) *
 *            binomial(k,2*n-3*k)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 43, u64 n <= 95, u128 n <= 203,
 *            big any n
 * Helpers:   comb
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/comb.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t k_start = (n + 1) / 2;          /* ceil(n/2) */
    idx_t k_end   = (2 * n) / 3;          /* floor(2n/3) */

    if (k_start > k_end)
        return sum;                       /* e.g. n = 1 */

    for (idx_t k = k_start; k <= k_end; ++k)
    {
        idx_t r = 2 * n - 3 * k;          /* lower index for binomial(k, r) */
        seq_t termA = ((seq_t)(k + 2) * (k + 1)) / 2;   /* binomial(k+2,2) */
        seq_t termB = comb(k, r);                     /* binomial(k, r) */
        sum += termA * termB;
    }
    return sum;
}
