/*
 * A329378  Least common multiple of exponents of prime factors of A108951(n),
 *          where A108951 is fully multiplicative with a(prime(i)) = prime(i)#
 *          = Product_{i=1..i} A000040(i).
 * https://oeis.org/A329378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of suffix sums of exponents of prime factors of n
 *            (the exponents of the prime factors of A108951(n))
 * Range:     u8 n <= 9681, u16 n <= 9681, u32 n <= 9681, u64 n <= 9681,
 *            u128 n <= 9681, big any n
 * Helpers:   factor_u64, gcd, swap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/swap.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n < 2)
        return (seq_t)1;

    /* maximal number of distinct prime factors of a 64‑bit integer < 20 */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   cnt = 0;

    /* factorise n */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* ensure primes are sorted in ascending order */
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (primes[j] < primes[i])
                swap(&primes[i], &primes[j], &exps[i], &exps[j]);

    seq_t lcm = (seq_t)1;
    uint64_t sum = 0;

    /* iterate from largest prime to smallest to accumulate suffix sums */
    size_t i = cnt;
    while (i--)
    {
        sum += exps[i];
        seq_t s = (seq_t)sum;
        seq_t g = gcd(lcm, s);
        lcm = (lcm / g) * s;
    }

    return lcm;
}
