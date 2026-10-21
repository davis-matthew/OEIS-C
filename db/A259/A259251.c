/*
 * A259251  a(n) = 1 + sigma(n) + sigma(n)^2 + sigma(n)^3 + sigma(n)^4 +
 *          sigma(n)^5 + sigma(n)^6.
 * https://oeis.org/A259251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + sigma(n) + sigma(n)^2 + sigma(n)^3 + sigma(n)^4 +
 *            sigma(n)^5 + sigma(n)^6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 19, u64 n <= 539,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t sum   = 0;
    seq_t power = 1;           /* power = sigma^k, starting with k=0 */
    for (idx_t i = 0; i < 7; ++i)
    {
        sum += power;
        power *= sigma;
    }
    return sum;
}
