/*
 * A379238  a(n) = 1 if A003961(n)-sigma(n) is prime, otherwise 0, where
 *          A003961 is fully multiplicative with a(prime(i)) = prime(i+1), and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A379238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A003961(n) - sigma(n) is prime, else 0 * where
 *            A003961 is fully multiplicative with a(prime(i)) = prime(i+1) *
 *            and sigma is the sum of divisors function
 * Range:     u8 n <= 47, u16 n <= 9412, u32 n <= 9412, u64 n <= 9412,
 *            u128 n <= 9412, big any n
 * Helpers:   is_prime_big, next_prime, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/seq_pow.h"
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n <= 2)
        return 0;

    /* Factorize n using trial division (sufficient for n ≤ 10000) */
    uint64_t temp = (uint64_t)n;
    seq_t prod = 1;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            unsigned exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            uint64_t next_p = next_prime(p);
            seq_t val = seq_pow((seq_t)next_p, exp);
            prod *= val;
        }
    }
    if (temp > 1) /* remaining prime factor */
    {
        uint64_t next_p = next_prime(temp);
        prod *= seq_pow((seq_t)next_p, 1);
    }

    seq_t sigma = sigma_of(n);
    seq_t diff = prod - sigma;

    if (diff <= (seq_t)1)
        return 0;

    return is_prime_big(diff) ? 1 : 0;
}
