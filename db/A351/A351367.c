/*
 * A351367  a(n) = Sum_{p|n, p prime} p^nextprime(p), where nextprime(n) is
 *          the smallest prime > n.
 * https://oeis.org/A351367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} p^nextprime(p)
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 10, u64 n <= 16, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t nextp = next_prime(p);
        seq_t powv = big_pow((seq_t)p, (seq_t)nextp);
        result += powv;
    }

    return result;
}
