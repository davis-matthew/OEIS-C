/*
 * A389460  a(n) = sigma(n) - A075423(n).
 * https://oeis.org/A389460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) – A075423(n) = sigma(n) – (∏p|n p – 1) (product
 *            of the distinct prime factors of n, minus one)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)                 /* defined only for n >= 1 */
        return 0;

    /* sum of divisors of n */
    seq_t sigma = sigma_of(n);

    /* compute the product of distinct prime factors of n */
    seq_t prod = 1;                 /* empty product = 1 */
    uint64_t temp = n;

    for (idx_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            prod = prod * (seq_t)i;
            while (temp % i == 0)
                temp /= i;
        }
    }
    if (temp > 1)                   /* remaining prime factor > sqrt(n) */
        prod = prod * (seq_t)temp;

    /* a(n) = sigma – prod + 1 */
    return sigma - prod + 1;
}
