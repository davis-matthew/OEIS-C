/*
 * A345281  a(n) = Sum_{p|n} nextprime(n/p), where nextprime(m) is the
 *          smallest prime > m.
 * https://oeis.org/A345281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} nextprime(n/p)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Factor n into distinct primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t prime = primes[i];
        uint64_t m = (uint64_t)n / prime;
        uint64_t np = next_prime(m);               /* smallest prime > m */
        sum += (seq_t)np;
    }
    return sum;
}
