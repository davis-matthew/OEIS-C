/*
 * A362522  a(n) = n! * Sum_{k=0..floor(n/2)} (k+1)^(k-1) / (k! * (n-2*k)!).
 * https://oeis.org/A362522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+1)^(k-1) / (k! * (n-2*k)!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small integer power for seq_t base and idx_t exponent.
   Exponents are at most n-1 (≤ 9999 for n ≤ 10000) and fit in idx_t. */
static inline seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum   = 0;
    seq_t factor = 1;                /* n!/(k!*(n-2*k)!) for current k */
    idx_t maxk  = n / 2;             /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t powv;
        if (k == 0)
            powv = 1;                /* convention: 1^(-1) = 1 */
        else
            powv = power_seq((seq_t)(k + 1), (idx_t)(k - 1));

        sum += factor * powv;

        if (k < maxk)
        {
            seq_t num1 = (seq_t)(n - 2 * k);
            seq_t num2 = (seq_t)(n - 2 * k - 1);
            seq_t denom = (seq_t)(k + 1);
            factor = factor * num1 * num2 / denom;
        }
    }

    return sum;
}
