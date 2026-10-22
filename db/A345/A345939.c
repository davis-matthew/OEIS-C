/*
 * A345939  a(n) = (n-1) / gcd(n-1, uphi(n)), where uphi is unitary totient
 *          (or unitary phi) function, A047994.
 * https://oeis.org/A345939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1) / gcd(n-1, uphi(n)), where uphi(n) is the unitary
 *            totient function A047994
 * Range:     u8 n <= 257, u16 n <= 9687, u32 n <= 9687, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;             /* a(1) = 0 */

    /* Lazily maintain a list of primes up to the largest n seen */
    static idx_t *primes = NULL;        /* idx_t is uint64_t */
    static size_t primes_cnt = 0;
    static idx_t prime_limit = 0;

    if (n > prime_limit) {
        if (primes) free(primes);
        build_primes(n, (idx_t **)&primes, &primes_cnt);
        prime_limit = n;
    }

    /* Compute the unitary totient uphi(n) = Π (p^e - 1) for each prime power p^e|n */
    seq_t uphi = 1;
    uint64_t temp = (uint64_t)n;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            /* count exponent e of p in n */
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* compute p^e - 1 */
            seq_t term = 1;
            for (int k = 0; k < e; ++k) term *= (seq_t)p;
            term -= 1;
            uphi *= term;
        }
    }

    /* If a prime factor greater than sqrt(n) remains */
    if (temp > 1) {
        seq_t term = (seq_t)temp - 1;
        uphi *= term;
    }

    /* a(n) = (n-1) / gcd(n-1, uphi(n)) */
    seq_t num = (seq_t)(n - 1);
    seq_t d   = gcd(num, uphi);
    return num / d;
}
