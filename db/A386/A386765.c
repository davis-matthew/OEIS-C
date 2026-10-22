/*
 * A386765  a(n) = Sum_{k=0..n} 5^k * 3^(n-k) * binomial(3*n+k-1,k).
 * https://oeis.org/A386765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 3^(n-k) * binomial(3*n + k - 1, k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"    /* binomial(idx_t n, idx_t k) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    /* Handle the offset: a(0) = 1 */
    if (n == 0)
        return 1;

    seq_t result = 0;
    seq_t pow5   = 1;            /* 5^k   */
    seq_t pow3   = seq_pow((seq_t)3, n);  /* 3^(n-k), start with k=0 */
    idx_t nn = 3 * n;            /* 3*n */

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(nn + k - 1, k);   /* C(3*n + k - 1, k) */
        seq_t term = bin * pow5 * pow3;
        result += term;

        pow5 *= 5;               /* next 5^k */
        pow3 /= 3;               /* next 3^(n-k-1) */
    }

    return result;
}
