/*
 * A333177  a(n) = Sum_{d|n, gcd(d, n/d) = 1} (-1)^omega(n/d) * prime(d).
 * https://oeis.org/A333177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n, gcd(d, n/d) = 1} (-1)^omega(n/d) * prime(d)
 *            where omega(m) is the number of distinct prime factors of m and
 *            prime(k) is the k-th prime (prime(1)=2)
 * Range:     u8 n <= 58, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes, gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 as safety */

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i)
    {
        if ((n % i) != 0)
            continue;

        idx_t d = i;
        idx_t e = n / i;

        /* coprime test */
        if (gcd((seq_t)d, (seq_t)e) == (seq_t)1)
        {
            /* contribution from divisor d */
            int omega_e = count_distinct_primes(e);
            seq_t prime_d = get_prime(d);
            if (omega_e & 1)
                sum -= prime_d;
            else
                sum += prime_d;

            /* if d != e, process the pair (e,d) separately */
            if (d != e && gcd((seq_t)e, (seq_t)d) == (seq_t)1)
            {
                int omega_d = count_distinct_primes(d);
                seq_t prime_e = get_prime(e);
                if (omega_d & 1)
                    sum -= prime_e;
                else
                    sum += prime_e;
            }
        }
    }

    return sum;
}
