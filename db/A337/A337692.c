/*
 * A337692  a(n) is the sum of all positive integers whose digits in base n
 *          are strictly decreasing.
 * https://oeis.org/A337692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{d=1}^{n-1} d * 2^{n-1-d} * (n+1)^d (base‑n strictly
 *            decreasing digits)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)                  /* a(1) = 0 by definition */
        return (seq_t)0;

    seq_t np1 = (seq_t)(n + 1);                  /* n+1 */
    seq_t total_pow2 = big_pow((seq_t)2, (seq_t)(n - 1)); /* 2^{n-1} */

    seq_t coeff = total_pow2 / (seq_t)2;         /* 2^{n-1-1}  for d = 1 */
    seq_t pow_np1 = np1;                         /* (n+1)^1   */
    seq_t sum = 0;

    for (idx_t d = 1; d < n; ++d)
    {
        seq_t term = (seq_t)d * pow_np1 * coeff;
        sum += term;

        pow_np1 *= np1;          /* (n+1)^{d+1} for next iteration */
        coeff /= (seq_t)2;       /* divide by 2 to get 2^{n-1-(d+1)} */
    }

    return sum;
}
