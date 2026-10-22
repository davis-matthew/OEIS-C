/*
 * A352985  a(n) = Sum_{k=0..floor(n/2)} k^(2*(n-2*k)).
 * https://oeis.org/A352985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^(2*(n-2*k))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max = n >> 1;                     /* floor(n/2) */
    for (idx_t k = 0; k <= max; ++k)
    {
        if (k == 0)                         /* handle 0^0 */
        {
            if (n == 0)
                sum += 1;                  /* 0^0 = 1 */
            /* else 0^positive = 0, no addition needed */
            continue;
        }
        seq_t base = (seq_t)k;
        seq_t exp  = 2 * ((seq_t)n - 2 * base);
        seq_t term = big_pow(base, exp);
        sum += term;
    }
    return sum;
}
